#pragma once

#include <deque>
#include <cstddef>

#include "Kitchen.hpp"
#include "Order.hpp"


class Simulation {
    private:
    Minutes currentTick = 0;
    std::deque<Order> orders;
    Kitchen kitchen;
    

    public:
    explicit Simulation(const Kitchen& kitchen_) : kitchen(kitchen_) {}

    Minutes getCurrentTick() const {
        return currentTick;
    }

    void nextTick() {
        ++currentTick;
        kitchen.finishCooking(currentTick);
        tryStartCookingDish();
    }

    const std::deque<Order>& getViewOrders() const {
        return orders;
    }

    void addOrder(const Order& order) {
        orders.push_back(order);
    }

    std::size_t getOrderCount() const {
        return orders.size();
    }

    void tryStartCookingDish() {
        for (auto& order : orders) {
            for (std::size_t i = 0; i < order.getOrderDishes().size(); i++) {
                Dish& dish = order.getDish(i);
                if (dish.getStatus() == DishStatus::Waiting) {
                    kitchen.TryStartCooking(dish, currentTick);
                }
            }
        }
    }
};