#include <iostream>

#include "Order.hpp"

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
    std::cout << salat.getCookingTime() << '\n';
    
    for (const Dish& dish : order1.getOrderDishes()) {
        std::cout << dish.getRecipe().getName() << '\n';
    }

    return 0;
}