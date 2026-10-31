#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <cstddef>
#include <limits>

#include "Dish.hpp"


class Order {
    private:
    std::int64_t id;
    std::vector<Dish> dishes;
    std::int64_t deadline;

    public:
    Order(std::int64_t id_, const std::vector<Dish>& orderDishes, std::int64_t dd = std::numeric_limits<std::int64_t>::max()) : 
    id(id_), dishes(orderDishes), deadline(dd) {
        if (id_ <= 0) {
            throw std::invalid_argument("ID заказа должен быть положительным");
        }
        if (orderDishes.empty()) {
            throw std::invalid_argument("Заказ не может быть пустым");
        }
        if (dd < 0) {
            throw std::invalid_argument("Дедлайн заказа не может быть отрицательным");
        }
    }
    std::int64_t getOrderId() const {
        return id;
    }
    const std::vector<Dish>& getOrderDishes() const {
        return dishes;
    }
    std::int64_t getDeadline() const {
        return deadline;
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
            throw std::out_of_range("Неверный индекс блюда");
        }

        dishes[index].setStatus(newstatus);
    }

    Dish& getDish(std::size_t index) {
        return dishes.at(index);
    }
};