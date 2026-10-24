#pragma once

#include <cstdint>
#include <map>
#include <stdexcept>

#include "Ingredient.hpp"

using Grams = std::int64_t;

class Recipe {
private:
    std::string name;
    std::map<Ingredient, Grams> ingredients;
    std::int64_t cookingTime;

public:
    Recipe(std::string n, const std::map<Ingredient, Grams>& ing, std::int64_t cookTime) : name(n), ingredients(ing), cookingTime(cookTime) {
        if (n.empty()) {
            throw std::invalid_argument("Recipe name can not be empty");
        }
        if (ing.empty()) {
            throw std::invalid_argument("Recipe can not be empty");
        }
        if (cookTime <= 0) {
            throw std::invalid_argument("CookingTime must be > 0");
        }
        for (const auto& [ingredient_, grams] : ing) {
            if (grams <= 0) {
                throw std::invalid_argument("Ingredient mass must be > 0");
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