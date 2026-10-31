#pragma once

#include <cstdint>
#include <stdexcept>
#include <map>
#include <vector>

#include "Dish.hpp"

using Minutes = std::int64_t;


class Cook {
    private:
    std::int64_t id;
    bool busy = false;

    public:
    Cook(std::int64_t id_) : id(id_) {
        if (id_ <= 0) {
            throw std::invalid_argument("ID повара должен быть положительным");
        }
    }
    std::int64_t getCookId() const {
        return id;
    }
    bool isBusy() const {
        return busy;
    }
    void startCooking() {
        if (busy) {
            throw std::logic_error("Повар уже занят");
        }
        busy = true;
    }
    void finishCooking() {
        if (!busy) {
            throw std::logic_error("Повар уже свободен");
        }
        busy = false;
    }
};


class Workplace {
    private:
    std::int64_t id;
    bool busy = false;

    public:
    Workplace(std::int64_t id_) : id(id_) {
        if (id_ <= 0) {
            throw std::invalid_argument("ID рабочего места должен быть положительным");
        }
    }
    std::int64_t getWorkplaceId() const {
        return id;
    }
    bool isBusy() const {
        return busy;
    }
    void occupy() {
        if (busy) {
            throw std::logic_error("Рабочее место уже занято");
        }
        busy = true;
    }
    void release() {
        if (!busy) {
            throw std::logic_error("Рабочее место уже свободно");
        }
        busy = false;
    }
};


class Storage {
    private:
    std::map<Ingredient, Grams> store;

    public:
    Storage(const std::map<Ingredient, Grams>& store_) : store(store_) {
        for (const auto& [ingredient, grams]: store_) {
            if (grams < 0) {
                throw std::invalid_argument("Кол-во грамм не может быть отрицательным");
            }
        }
    }
    const std::map<Ingredient, Grams>& getStore() const {
        return store;
    }

    bool hasIngredients(const Recipe& recipe) const {
        for (const auto& [ingredient, grams] : recipe.getIngredients()) {
            if (!store.contains(ingredient)) {
                return false;
            }
            else if (store.at(ingredient) < grams) {
                return false;
            }
        }
        return true;
    }

    void consumeIngredients(const Recipe& recipe) {
        if (!hasIngredients(recipe)) {
            throw std::logic_error("Недостаточно ингредиентов на складе");
        }
        for (const auto& [ingredient, grams] : recipe.getIngredients()) {
            store.at(ingredient) -= grams;
        }
    }
    void restock(Ingredient ingredient, Grams grams) {
        if (grams <= 0) {
            throw std::invalid_argument("Число грамм должно быть положительным");
        }
        store[ingredient] += grams;
    }

};


struct CookingTask {
    Dish* dish;
    Cook* cook;
    Workplace* workplace;
    Minutes finishTick;
};


class Kitchen {
    private:
    std::vector<CookingTask> activeTasks;
    std::vector<Cook> cooks;
    std::vector<Workplace> workplaces;
    Storage storage;

    public:
    Kitchen(const std::vector<Cook>& cooks_, const std::vector<Workplace>& places_, const Storage& stor) : 
    cooks(cooks_), workplaces(places_), storage(stor) {}

    Cook* findFreeCook() {
        for (auto& cook : cooks) {
            if (!cook.isBusy()) {
                return &cook;
            }
        }
        return nullptr;
    }

    Workplace* findFreePlace() {
        for (auto& place : workplaces) {
            if (!place.isBusy()) {
                return &place;
            }
        }
        return nullptr;
    }

    bool TryStartCooking(Dish& dish, Minutes currentTick) {
        if (dish.getStatus() != DishStatus::Waiting) {
            throw std::logic_error("Блюдо должно иметь статус Waiting");
        }
        Cook* cook = findFreeCook();
        Workplace* workplace = findFreePlace();
        if (cook == nullptr || workplace == nullptr) {
            return false;
        }
        if (!storage.hasIngredients(dish.getRecipe())) {
            return false;
        }

        storage.consumeIngredients(dish.getRecipe());
        cook->startCooking();
        workplace->occupy();
        dish.setStatus(DishStatus::Cooking);

        Minutes finishTick = currentTick + dish.getRecipe().getCookingTime();
        activeTasks.push_back({&dish, cook, workplace, finishTick});

        return true;
    }

    void finishCooking(Minutes currentTick) {
        for (std::size_t i = activeTasks.size(); i > 0; --i) {
            auto& task = activeTasks[i-1];
            if (task.finishTick <= currentTick) {
                task.dish->setStatus(DishStatus::Ready);
                task.cook->finishCooking();
                task.workplace->release();

                activeTasks.erase(activeTasks.begin() + (i - 1));
            }
        }
    }

    void restock(Ingredient ingredient, Grams grams) {
        storage.restock(ingredient, grams);
    }
};