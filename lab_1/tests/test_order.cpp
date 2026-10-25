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

    order.SetDishStatus(0, DishStatus::Cooking);
    EXPECT_FALSE(order.isReady());
    EXPECT_THROW(order.SetDishStatus(1, DishStatus::Ready), std::logic_error);
    order.SetDishStatus(1, DishStatus::Cooking);
    EXPECT_FALSE(order.isReady());
    order.SetDishStatus(0, DishStatus::Ready);
    EXPECT_FALSE(order.isReady());
    order.SetDishStatus(1, DishStatus::Ready);
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

    EXPECT_THROW(order.SetDishStatus(1, DishStatus::Cooking), std::out_of_range);
    EXPECT_EQ(order.getDish(0).getStatus(), DishStatus::Waiting);
    EXPECT_THROW(order.getDish(67), std::out_of_range);
}
