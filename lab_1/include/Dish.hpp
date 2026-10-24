#pragma once

#include <stdexcept>
#include "Recipe.hpp"

enum class DishStatus {
    Waiting,
    Cooking,
    Ready
};

class Dish {
    private:
    Recipe recipe;
    DishStatus status = DishStatus::Waiting;

    public:
    Dish(const Recipe& rec) : recipe(rec) {}

    const Recipe& getRecipe() const {
        return recipe;
    }

    DishStatus getStatus() const {
        return status;
    }
    void setStatus(DishStatus newStatus) {
        if (status == DishStatus::Waiting && newStatus == DishStatus::Cooking) {
            status = newStatus;
        }
        else if (status == DishStatus::Cooking && newStatus == DishStatus::Ready) {
            status = newStatus;
        }
        else {
            throw std::logic_error("Invalid dish status transition");
        }
    }
};
