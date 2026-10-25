#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <cstddef>
#include "Dish.hpp"


class Order {
    private:
    std::int64_t id;
    std::vector<Dish> dishes;

    public:
    Order(std::int64_t id_, const std::vector<Dish>& orderDishes) : id(id_), dishes(orderDishes) {
        if (id_ <= 0) {
            throw std::invalid_argument("Order Id must be > 0");
        }
        if (orderDishes.empty()) {
            throw std::invalid_argument("Order can not be empty");
        }
    }
    std::int64_t getOrderId() const {
        return id;
    }
    const std::vector<Dish>& getOrderDishes() const {
        return dishes;
    }
    bool isReady() const {
        for (const Dish& dish: dishes) {
            if (dish.getStatus() != DishStatus::Ready) {
                return false;
            }
        }
        return true;
    }

    void SetDishStatus(std::size_t index, DishStatus newstatus) {
        if (index >= dishes.size()) {
            throw std::out_of_range("Dish index is out of range");
        }

        dishes[index].setStatus(newstatus);
    }

    Dish& getDish(std::size_t index) {
        return dishes.at(index);
    }
};