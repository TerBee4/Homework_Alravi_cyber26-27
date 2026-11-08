#include "Scheduler.hpp"
#include <gtest/gtest.h>


TEST(SchedulerTest, FIFO) {
    Recipe recipe("Pizza", {{Ingredient::Flour, 42}}, 7);

    std::deque<Order> orders;
    orders.emplace_back(1, std::vector<Dish>{Dish(recipe)}, 30);
    orders.emplace_back(2, std::vector<Dish>{Dish(recipe)}, 10);
    orders.emplace_back(3, std::vector<Dish>{Dish(recipe)}, 20);

    Scheduler scheduler;

    EXPECT_EQ(scheduler.getPolicy(), SchedulingPolicy::FIFO);

    std::vector<std::size_t> indices = scheduler.getOrderIndeces(orders);

    EXPECT_EQ(indices, (std::vector<std::size_t>{0, 1, 2}));
}


TEST(SchedulerTest, EDF) {
    Recipe recipe("Pizza", {{Ingredient::Flour, 42}}, 7);

    std::deque<Order> orders;
    orders.emplace_back(1, std::vector<Dish>{Dish(recipe)}, 30);
    orders.emplace_back(2, std::vector<Dish>{Dish(recipe)}, 10);
    orders.emplace_back(3, std::vector<Dish>{Dish(recipe)}, 10);
    orders.emplace_back(4, std::vector<Dish>{Dish(recipe)}, 20);

    Scheduler scheduler;
    scheduler.setPolicy(SchedulingPolicy::EDF);

    EXPECT_EQ(scheduler.getPolicy(), SchedulingPolicy::EDF);

    std::vector<std::size_t> indices = scheduler.getOrderIndeces(orders);

    EXPECT_EQ(indices, (std::vector<std::size_t>{1, 2, 3, 0}));
}