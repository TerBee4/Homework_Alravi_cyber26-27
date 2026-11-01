#include <gtest/gtest.h>
#include "OrderGenerator.hpp"


TEST(OrderGeneratorTest, ProbabilityCheck) {
    std::vector<Recipe> menu = {
        Recipe("Pizza", {{Ingredient::Flour, 200}}, 8)
    };

    OrderGenerator no(42, 0.0, menu);
    OrderGenerator yes(42, 1.0, menu);

    for (int i = 0; i < 10; ++i) {
        EXPECT_FALSE(no.mayGenerate());
        EXPECT_TRUE(yes.mayGenerate());
    }
}

TEST(OrderGeneratorTest, GeneratedOrderValid) {
    std::vector<Recipe> menu = {
        Recipe("Pizza", {{Ingredient::Flour, 200}}, 8)
    };

    OrderGenerator generator(42, 1.0, menu);
    Order order = generator.generateOrder(5, 7);
    EXPECT_EQ(order.getOrderId(), 7);

    const auto& dishes = order.getOrderDishes();
    EXPECT_GE(dishes.size(), 1);
    EXPECT_LE(dishes.size(), 3);

    std::int64_t cookingTime = 0;

    for (const Dish& dish : dishes) {
        cookingTime += dish.getRecipe().getCookingTime();
    }
    EXPECT_EQ(order.getDeadline(), 5 + cookingTime + 10);
}