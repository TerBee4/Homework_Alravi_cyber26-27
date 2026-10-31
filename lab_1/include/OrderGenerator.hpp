#pragma once

#include <stdexcept>
#include <random>
#include <cstdint>
#include <vector>
#include "Order.hpp"

using Minutes = std::int64_t;


class OrderGenerator {
    private:
    std::mt19937 generator;
    double probability;
    std::vector<Recipe> recipes;

    public:
    OrderGenerator(std::uint32_t seed, double probab, std::vector<Recipe>& rec) : generator(seed), probability(probab), recipes(rec) {
        if (probab < 0.0 || probab > 1.0) {
            throw std::invalid_argument("Неверная вероятность заказа");
        }
        if (rec.empty()) {
            throw std::invalid_argument("Список рецептов не может быть пустым");
        }
    }
    bool mayGenerate() {
        std::bernoulli_distribution distribution(probability);
        return distribution(generator);
    }

    Order generateOrder(Minutes currentTick, std::int64_t OrderId) {
        std::uniform_int_distribution<int> amountDistribution(1, 3);
        int dishCount = amountDistribution(generator);

        std::vector<Dish> dishes;

        std::uniform_int_distribution<std::size_t> recipeDistribution(0, recipes.size() - 1);
        for (int i = 0; i < dishCount; ++i) {
            std::size_t index = recipeDistribution(generator);
            dishes.emplace_back(recipes[index]);
        }

        Minutes totalCookingTime = 0;

        for (const Dish& dish : dishes) {
            totalCookingTime += dish.getRecipe().getCookingTime();
        }

        Minutes deadline = currentTick + totalCookingTime + 10;
        return Order(OrderId, dishes, deadline);
    }
};