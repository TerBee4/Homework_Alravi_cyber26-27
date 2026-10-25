#include <iostream>

#include "Order.hpp"
#include "Kitchen.hpp"
#include "Simulation.hpp"

int main() {
    Recipe pizzaRecipe(
        "Pizza",
        {{Ingredient::Flour, 200},
         {Ingredient::Tomato, 100},
         {Ingredient::Cheese, 100}},
        15
    );

    Recipe omeletteRecipe(
        "Omelette",
        {{Ingredient::Egg, 150},
         {Ingredient::Milk, 70},
         {Ingredient::Bread, 50}},
        7
    );

    Dish pizza(pizzaRecipe);
    Dish omelette(omeletteRecipe);

    Storage storage({
        {Ingredient::Flour, 200},
        {Ingredient::Tomato, 100},
        {Ingredient::Cheese, 100},
        {Ingredient::Egg, 150},
        {Ingredient::Milk, 70},
        {Ingredient::Bread, 50}
    });

    Kitchen kitchen(
        {Cook(1), Cook(2)},
        {Workplace(1), Workplace(2)},
        storage
    );


    std::cout << "Pizza started: "
              << kitchen.TryStartCooking(pizza, 0) << '\n';

    std::cout << "Omelette started: "
              << kitchen.TryStartCooking(omelette, 0) << '\n';

    for (Minutes tick = 0; tick <= 15; ++tick) {
        kitchen.finishCooking(tick);

        if (tick == 0 || tick == 7 || tick == 15) {
            std::cout << "\nTick: " << tick << '\n';

            std::cout << "Pizza status: "
                      << static_cast<int>(pizza.getStatus()) << '\n';

            std::cout << "Omelette status: "
                      << static_cast<int>(omelette.getStatus()) << '\n';

            std::cout << "Free cook: "
                      << (kitchen.findFreeCook() != nullptr) << '\n';

            std::cout << "Free workplace: "
                      << (kitchen.findFreePlace() != nullptr) << '\n';
        }
    }

    Simulation simulation(kitchen);

    simulation.addOrder(Order(1, {pizza}));
    simulation.addOrder(Order(2, {omelette}));

    std::cout << "Orders count: " << simulation.getOrderCount() << '\n';

    for (const auto& order : simulation.getViewOrders()) {
        std::cout << "Order #" << order.getOrderId() << '\n';
        for (const auto& dish : order.getOrderDishes()) {
            std::cout << dish.getRecipe().getName() << ": " << static_cast<int>(dish.getStatus()) << '\n';
        }
    }


    // std::cout << "\nSimulation tick: "
    //       << simulation.getCurrentTick() << '\n';

    // for (int i = 0; i < 7; ++i) {
    //     simulation.nextTick();
    // }

    // std::cout << "Simulation tick: "
    //       << simulation.getCurrentTick() << '\n';


    // Recipe pizza("Pizza", {
    //     {Ingredient::Flour, 200},
    //     {Ingredient::Tomato, 100},
    //     {Ingredient::Cheese, 100}
    // }, 15);

    // Recipe salat("Cesar", {
    //     {Ingredient::Greens, 200},
    //     {Ingredient::Egg, 100},
    //     {Ingredient::Bread, 100}
    // }, 7);

    // Dish dish1(pizza);
    // Dish Salat(salat);
    // Order order1(1, {dish1, Salat});

    // Storage storage({
    //     {Ingredient::Flour, 500}
    // });

    // storage.restock(Ingredient::Flour, 200);
    // storage.restock(Ingredient::Tomato, 200);
    // storage.restock(Ingredient::Cheese, 300);

    // Cook cook1(1);
    // Workplace place1(1);

    // Kitchen kyhna1({cook1}, {place1}, storage);

    // bool started = kyhna1.TryStartCooking(dish1);

    // std::cout << "Started: " << started << '\n';
    // std::cout << "Cooking: "
    //           << (dish1.getStatus() == DishStatus::Cooking) << '\n';
    // std::cout << "Free cook: "
    //           << (kyhna1.findFreeCook() != nullptr) << '\n';
    // std::cout << "Free workplace: "
    //           << (kyhna1.findFreePlace() != nullptr) << '\n';



    // for (const auto& [ingredient, grams] : storage.getStore()) {
    //     std::cout << grams << '\n';
    // }

    // try {
    //     storage.consumeIngredients(pizza);
    // } catch (const std::logic_error& error) {
    //     std::cout << error.what() << '\n';
    // }

    // std::cout << "Flour: "
    //         << storage.getStore().at(Ingredient::Flour) << '\n';

    // std::cout << "Cheese: "
    //         << storage.getStore().at(Ingredient::Cheese) << '\n';


    //std::cout << salat.getCookingTime() << '\n';
    
    //for (const Dish& dish : order1.getOrderDishes()) {
    //    std::cout << dish.getRecipe().getName() << '\n';
    //}

    return 0;
}