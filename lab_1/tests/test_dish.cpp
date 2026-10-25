#include <gtest/gtest.h>
#include "Dish.hpp"

TEST(RecipeTest, StoresRecipeData) {
    Recipe recipe(
        "Omelette",
        {
            {Ingredient::Egg, 150},
            {Ingredient::Milk, 70}
        },
        7
    );

    EXPECT_EQ(recipe.getName(), "Omelette");
    EXPECT_EQ(recipe.getCookingTime(), 7);
    EXPECT_EQ(recipe.getIngredients().size(), 2);
}

TEST(DishTest, getRecipe) {
    Recipe recipe(
        "Salat",
        {
            {Ingredient::Egg, 70},
            {Ingredient::Greens, 200},
            {Ingredient::Bread, 100}
        },
        10
    );

    Dish dish(recipe);

    EXPECT_EQ(dish.getRecipe().getName(), recipe.getName());
    EXPECT_EQ(dish.getRecipe().getCookingTime(), recipe.getCookingTime());
    EXPECT_EQ(dish.getRecipe().getIngredients(), recipe.getIngredients());
    EXPECT_EQ(dish.getStatus(), DishStatus::Waiting);
}

TEST(DishTest, settingStatus) {
    Recipe recipe(
        "Salat",
        {
            {Ingredient::Egg, 70},
            {Ingredient::Greens, 200},
            {Ingredient::Bread, 100}
        },
        10
    );

    Dish dish(recipe);

    EXPECT_EQ(dish.getStatus(), DishStatus::Waiting);
    dish.setStatus(DishStatus::Cooking);
    EXPECT_EQ(dish.getStatus(), DishStatus::Cooking);
    dish.setStatus(DishStatus::Ready);
    EXPECT_EQ(dish.getStatus(), DishStatus::Ready);
    
}

TEST(DishTest, CannotSkipCooking) {
    Recipe recipe(
        "Salat",
        {
            {Ingredient::Egg, 70},
            {Ingredient::Greens, 200},
            {Ingredient::Bread, 100}
        },
        10
    );

    Dish dish(recipe);

    EXPECT_THROW(dish.setStatus(DishStatus::Ready), std::logic_error);
    EXPECT_EQ(dish.getStatus(), DishStatus::Waiting);
}

TEST(DishTest, BackToWaiting) {
    Recipe recipe(
        "Salat",
        {
            {Ingredient::Egg, 70},
            {Ingredient::Greens, 200},
            {Ingredient::Bread, 100}
        },
        10
    );

    Dish dish(recipe);
    dish.setStatus(DishStatus::Cooking);

    EXPECT_THROW(dish.setStatus(DishStatus::Waiting), std::logic_error);
    EXPECT_EQ(dish.getStatus(), DishStatus::Cooking);
    dish.setStatus(DishStatus::Ready);
    EXPECT_THROW(dish.setStatus(DishStatus::Waiting), std::logic_error);
    EXPECT_EQ(dish.getStatus(), DishStatus::Ready);
}

