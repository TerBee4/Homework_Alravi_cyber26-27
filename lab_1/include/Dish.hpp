#pragma once

#include <cstdint>
#include <map>
#include <stdexcept>


enum class Ingredient {
    Flour,
    Tomato,
    Cheese,
    Egg,
    Cucumber,
    Bread,
    Greens,
    Potato,
    Meat,
    Carrot,
    Rice,
    Fish,
    Sugar,
    CreamCheese,
    Milk
};

enum class DishStatus {
    Waiting,
    Cooking,
    Ready
};

using Grams = std::int64_t;
using Minutes = std::int64_t;

class Recipe {
    private:
        std::string name;
        std::map<Ingredient, Grams> ingredients;
        std::int64_t cookingTime;

    public:
        Recipe(std::string n, const std::map<Ingredient, Grams>& ing, std::int64_t cookTime) : 
        name(n), ingredients(ing), cookingTime(cookTime) {
            if (n.empty()) {
                throw std::invalid_argument("Название рецепта не может быть пустым");
            }
            if (ing.empty()) {
                throw std::invalid_argument("Рецепт не может быть пустым");
            }
            if (cookTime <= 0) {
                throw std::invalid_argument("Времы приготовления должно быть положительным");
            }
            for (const auto& [ingredient_, grams] : ing) {
                if (grams <= 0) {
                    throw std::invalid_argument("Масса ингредиентов должна быть положительной");
                }
            }
        }
        const std::string& getName() const {
            return name;
        }
        const std::map<Ingredient, Grams>& getIngredients() const {
            return ingredients;
        }
        std::int64_t getCookingTime() const {
            return cookingTime;
        }
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

    void startCooking() {
        if (status == DishStatus::Waiting) {
            status = DishStatus::Cooking;
        }
        else throw std::logic_error("Блюдо нельзя начать готовить");
    }

    void finishCooking() {
        if (status == DishStatus::Cooking) {
            status = DishStatus::Ready;
        }
        else throw std::logic_error("Блюдо не находится в процессе приготовления");   
    }
};
