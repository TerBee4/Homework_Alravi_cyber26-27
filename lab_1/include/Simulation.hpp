#pragma once

#include <deque>
#include <cstddef>
#include <stdexcept>
#include <cstdint>
#include <optional>

#include "Kitchen.hpp"
#include "Order.hpp"
#include "Scheduler.hpp"
#include "OrderGenerator.hpp"


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
    

    public:
    explicit Simulation(const Kitchen& kitchen_) : kitchen(kitchen_) {}

    Minutes getCurrentTick() const {
        return currentTick;
    }

    void nextTick() {
        ++currentTick;
        if (orderGenerator.has_value()) {
            if (orderGenerator->mayGenerate()) {
                Order order = orderGenerator->generateOrder(currentTick, nextOrderId);
                addOrder(order);
            }
        }

        kitchen.finishCooking(currentTick);
        
        if (mode == SimulationMode::Automatic) {
            tryStartCookingDish();
        }
    }

    void setMode(SimulationMode newMode) {
        mode = newMode;
    }

    void setScheduler(SchedulingPolicy pol) {
        scheduler.setPolicy(pol);
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
                    kitchen.TryStartCooking(dish, currentTick);
                }
            }
        }
    }

    bool startDish(int orderId, std::size_t dishIndex) {
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                Dish& dish = order.getDish(dishIndex);
                return kitchen.TryStartCooking(dish, currentTick);
            }
        }
        throw std::out_of_range("Заказ не найден");
    }

    void restock(Ingredient ingredient, Grams grams) {
        kitchen.restock(ingredient, grams);
    }
};