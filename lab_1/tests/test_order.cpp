#include <gtest/gtest.h>

#include "Order.hpp"

TEST(OrderTest, CreatedOrder) {
    Recipe recipe(
        "Pizza",
        {
            {Ingredient::Flour, 200},
            {Ingredient::Cheese, 100},
            {Ingredient::Tomato, 100}
        },
        20
    );

    Dish pizza(recipe);
    Order order(1, {pizza});

    EXPECT_EQ(order.getOrderId(), 1);
    EXPECT_EQ(order.getOrderDishes().size(), 1);
    EXPECT_FALSE(order.isReady());
}

TEST(OrderTest, IsOrderReady) {
    Recipe recipe1(
        "Pizza",
        {
            {Ingredient::Flour, 200},
            {Ingredient::Cheese, 100},
            {Ingredient::Tomato, 100}
        },
        20
    );
    Recipe recipe2(
        "Salat",
        {
            {Ingredient::Egg, 70},
            {Ingredient::Greens, 200},
            {Ingredient::Bread, 100}
        },
        10
    );

    Dish pizza(recipe1);
    Dish salat(recipe2);
    Order order(1, {pizza, salat});

    order.getDish(0).startCooking();
    EXPECT_FALSE(order.isReady());
    EXPECT_THROW(order.getDish(1).finishCooking(), std::logic_error);
    order.getDish(1).startCooking();
    EXPECT_FALSE(order.isReady());
    order.getDish(0).finishCooking();
    EXPECT_FALSE(order.isReady());
    order.getDish(1).finishCooking();
    EXPECT_TRUE(order.isReady());
}

TEST(OrderTest, EmptyOrder) {
    EXPECT_THROW(Order order(1, {}), std::invalid_argument);
}

TEST(OrderTest, NegIdOrder) {
    Recipe recipe(
        "Salat",
        {
            {Ingredient::Egg, 70},
            {Ingredient::Greens, 200},
            {Ingredient::Bread, 100}
        },
        10
    );
    Dish salat(recipe);
    EXPECT_THROW(Order order(-1, {salat}), std::invalid_argument);
}

TEST(OrderTest, WrongDishOrder) {
    Recipe recipe(
        "Pizza",
        {{Ingredient::Flour, 200}},
        15
    );

    Dish pizza(recipe);
    Order order(1, {pizza});

    EXPECT_THROW(order.getDish(1).startCooking(), std::out_of_range);
    EXPECT_EQ(order.getDish(0).getStatus(), DishStatus::Waiting);
    EXPECT_THROW(order.getDish(67), std::out_of_range);
}
