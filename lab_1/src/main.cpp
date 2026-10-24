#include <iostream>

#include "Order.hpp"
#include "Kitchen.hpp"

int main() {
    Recipe pizza("Pizza", {
        {Ingredient::Flour, 200},
        {Ingredient::Tomato, 100},
        {Ingredient::Cheese, 100}
    }, 15);

    Recipe salat("Cesar", {
        {Ingredient::Greens, 200},
        {Ingredient::Egg, 100},
        {Ingredient::Bread, 100}
    }, 7);

    Dish Pizza(pizza);
    Dish Salat(salat);
    Order order1(1, {Pizza, Salat});

    Storage storage({
        {Ingredient::Flour, 500}
    });

    storage.restock(Ingredient::Flour, 200);
    storage.restock(Ingredient::Tomato, 200);
    storage.restock(Ingredient::Cheese, 20);


    for (const auto& [ingredient, grams] : storage.getStore()) {
        std::cout << grams << '\n';
    }

    try {
        storage.consumeIngredients(pizza);
    } catch (const std::logic_error& error) {
        std::cout << error.what() << '\n';
    }

    std::cout << "Flour: "
            << storage.getStore().at(Ingredient::Flour) << '\n';

    std::cout << "Cheese: "
            << storage.getStore().at(Ingredient::Cheese) << '\n';


    //std::cout << salat.getCookingTime() << '\n';
    
    //for (const Dish& dish : order1.getOrderDishes()) {
    //    std::cout << dish.getRecipe().getName() << '\n';
    //}

    return 0;
}