#include <gtest/gtest.h>

#include "Simulation.hpp"

TEST(SimulationTest, SingleOrderSim) {
    Storage storage({
        {Ingredient::Milk, 100},
        {Ingredient::Fish, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 2);
    Dish coctail(recipe);
    Order order(1, {coctail});
    Simulation sim(kitchen);
    sim.addOrder(order);

    EXPECT_EQ(sim.getOrderCount(), 1);
    EXPECT_EQ(sim.getCurrentTick(), -1);
    EXPECT_EQ(sim.getViewOrders().at(0).getOrderDishes().at(0).getStatus(), DishStatus::Waiting);
    sim.nextTick();
    EXPECT_EQ(sim.getCurrentTick(), 0);
    EXPECT_EQ(sim.getViewOrders().at(0).getOrderDishes().at(0).getStatus(), DishStatus::Cooking);
    sim.nextTick();
    sim.nextTick();
    EXPECT_EQ(sim.getCurrentTick(), 2);
    EXPECT_EQ(sim.getViewOrders().at(0).getOrderDishes().at(0).getStatus(), DishStatus::Ready);
    EXPECT_TRUE(sim.getViewOrders().at(0).isReady());
}