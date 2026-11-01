#include <gtest/gtest.h>
#include "Statistics.hpp"

TEST(StatisticsTest, RecordAndExport) {
    Statistics stats;
    Recipe recipe("Pizza", {{Ingredient::Flour, 100}}, 5);
    std::deque<Order> orders;

    orders.emplace_back(1, std::vector<Dish>{Dish(recipe)}, 20);

    stats.recordTick(0, orders, 0.0, 0.0);
    stats.recordCompletedOrder(true);
    stats.recordCompletedOrder(false);
    stats.recordTick(1, orders, 50.0, 100.0);

    const auto& history = stats.getStatisticsList();

    ASSERT_EQ(history.size(), 2);
    EXPECT_EQ(history[0].waitingOrders, 1);
    EXPECT_DOUBLE_EQ(history[0].onTimeProcent, 0.0);
    EXPECT_EQ(history[1].tick, 1);
    EXPECT_EQ(history[1].waitingOrders, 1);
    EXPECT_DOUBLE_EQ(history[1].onTimeProcent, 50.0);
    EXPECT_DOUBLE_EQ(history[1].cookUtilization, 50.0);
    EXPECT_DOUBLE_EQ(history[1].workplaceUtilization, 100.0);

    const std::string filename = "test_statistics.csv";
    stats.saveToCsv(filename);

    std::ifstream file(filename);
    EXPECT_TRUE(file.is_open());

    file.close();
    std::remove(filename.c_str());
}