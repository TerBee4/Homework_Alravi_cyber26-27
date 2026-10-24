#include <cstdint>
#include <stdexcept>
#include <map>


class Cook {
    private:
    std::int64_t id;
    bool busy = false;

    public:
    Cook(std::int64_t id_) : id(id_) {
        if (id_ <= 0) {
            throw std::invalid_argument("Cook Id must be > 0");
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
            throw std::logic_error("Cook is already busy");
        }
        busy = true;
    }
    void finishCooking() {
        if (!busy) {
            throw std::logic_error("Cook is already free");
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
            throw std::invalid_argument("Workplace Id must be > 0");
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
            throw std::logic_error("Workplace is already busy");
        }
        busy = true;
    }
    void release() {
        if (!busy) {
            throw std::logic_error("Workplace is already free");
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
                throw std::invalid_argument("grams can not be < 0");
            }
        }
    }
    const std::map<Ingredient, Grams>& getStore() const {
        return store;
    }

    void consumeIngredients(const Recipe& recipe) {
        for (const auto& [ingredient, grams] : recipe.getIngredients()) {
            if (!store.contains(ingredient)) {
                throw std::logic_error("Ingredient is missing");
            }
            else if (store.at(ingredient) < grams) {
                throw std::logic_error("Not enough ingredients at the storage");
            }
        }
        for (const auto& [ingredient, grams] : recipe.getIngredients()) {
            store.at(ingredient) -= grams;
        }
    }
    void restock(Ingredient ingredient, Grams grams) {
        if (grams <= 0) {
            throw std::invalid_argument("Grams amount must be > 0");
        }
        store[ingredient] += grams;
    }

};