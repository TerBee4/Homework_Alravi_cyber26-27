#pragma once

#include <deque>
#include <cstddef>
#include <stdexcept>
#include <cstdint>
#include <optional>
#include <string>

#include "Kitchen.hpp"
#include "Order.hpp"
#include "Scheduler.hpp"
#include "OrderGenerator.hpp"
#include "EventJournal.hpp"
#include "Statistics.hpp"


enum class SimulationMode {
    Manual,
    Automatic
};


class Simulation {
    private:
    Scheduler scheduler;
    SimulationMode mode = SimulationMode::Automatic;
    Minutes currentTick = -1;
    std::deque<Order> orders;
    Kitchen kitchen;
    std::int64_t nextOrderId = 1;
    std::optional<OrderGenerator> orderGenerator;
    EventJournal journal;
    Statistics statistics;

    public:
    explicit Simulation(const Kitchen& kitchen_) : kitchen(kitchen_) {}

    Minutes getCurrentTick() const {
        return currentTick;
    }

    const Statistics& getStatistics() const {
        return statistics;
    }

    void nextTick() {
        ++currentTick;
        if (orderGenerator.has_value()) {
            if (orderGenerator->mayGenerate()) {
                Order order = orderGenerator->generateOrder(currentTick, nextOrderId);
                addOrder(order);
            }
        }
        std::vector<std::pair<std::int64_t, Dish*>> cookingDishes;

        for (auto& order : orders) {
            for (std::size_t i = 0; i < order.getOrderDishes().size(); ++i) {
                Dish& dish = order.getDish(i);
                if (dish.getStatus() == DishStatus::Cooking) {
                    cookingDishes.emplace_back(order.getOrderId(), &dish);
                }
            }
        }
        std::vector<std::size_t> unfinishedOrders;

        for (std::size_t i = 0; i < orders.size(); ++i) {
            if (!orders[i].isReady()) {
                unfinishedOrders.push_back(i);
            }
        }

        kitchen.finishCooking(currentTick);

        for (const auto& [orderId, dish] : cookingDishes) {
            if (dish->getStatus() == DishStatus::Ready) {
                journal.record(currentTick, "Готово блюдо " + 
                dish->getRecipe().getName() + " (заказ №" + std::to_string(orderId) + ")");
            }
        }
        for (std::size_t index : unfinishedOrders) {
            if (orders[index].isReady()) {
                journal.record(currentTick, "Заказ №"
                     + std::to_string(orders[index].getOrderId()) + " полностью выполнен");
                statistics.recordCompletedOrder(currentTick <= orders[index].getDeadline());
            }
        }
        
        if (mode == SimulationMode::Automatic) {
            tryStartCookingDish();
        }
        statistics.recordTick(currentTick, orders, kitchen.getCookUtilization(), kitchen.getWorkPlaceUtilization());
    }

    void setMode(SimulationMode newMode) {
        mode = newMode;
    }
    SimulationMode getMode() const {
        return mode;
    }

    void setScheduler(SchedulingPolicy pol) {
        scheduler.setPolicy(pol);
    }
    SchedulingPolicy getSchedulingPolicy() const {
        return scheduler.getPolicy();
    }

    void setOrderGenerator(const OrderGenerator& generator) {
        orderGenerator = generator;
    }
    const std::deque<Order>& getViewOrders() const {
        return orders;
    }

    void addOrder(const Order& order) {
        for (const Order& ord : orders) {
            if (ord.getOrderId() == order.getOrderId()) {
                throw std::invalid_argument("Заказ с этим ID уже существует");
            }
        }

        orders.push_back(order);

        if (order.getOrderId() >= nextOrderId) {
            nextOrderId = order.getOrderId() + 1;
        }

        journal.record(currentTick, "Поступил заказ №" + std::to_string(order.getOrderId()));
    }

    std::size_t getOrderCount() const {
        return orders.size();
    }

    void tryStartCookingDish() {
        std::vector<std::size_t> indeces = scheduler.getOrderIndeces(orders);
        for (std::size_t index : indeces) {
            for (std::size_t i = 0; i < orders[index].getOrderDishes().size(); i++) {
                Dish& dish = orders[index].getDish(i);
                if (dish.getStatus() == DishStatus::Waiting) {
                    if(kitchen.TryStartCooking(dish, currentTick)) {
                        journal.record(currentTick, "Началось приготовление " + dish.getRecipe().getName()
                         + " (заказ №" + std::to_string(orders[index].getOrderId()) + ")");
                    };
                }
            }
        }
    }

    bool startDish(int orderId, std::size_t dishIndex) {
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                Dish& dish = order.getDish(dishIndex);
                if (kitchen.TryStartCooking(dish, currentTick)) {
                    journal.record(currentTick, "Началось приготовление " 
                        + dish.getRecipe().getName() + " (заказ № " + std::to_string(orderId) + ")");
                    return true;
                }
                return false;
            }
        }
        throw std::out_of_range("Заказ не найден");
    }

    void restock(Ingredient ingredient, Grams grams) {
        kitchen.restock(ingredient, grams);
        journal.record(currentTick, "Пополнение склада: ингредиент №" 
            + std::to_string(static_cast<int>(ingredient)) + ", количество: " + std::to_string(grams) + " г");
    }

    const EventJournal& getJournal() const {
        return journal;
    }
};