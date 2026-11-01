#include <gtest/gtest.h>
#include "EventJournal.hpp"
#include <cstdio>

TEST(EventJournalTest, SaveAndLoad) {
    EventJournal journal;

    journal.record(0, "Поступил заказ");
    journal.record(5, "Заказ выполнен");

    const std::string filename = "test_journal.txt";
    journal.saveToFile(filename);

    EventJournal loadedJournal;
    loadedJournal.loadFromFile(filename);

    const auto& entries = loadedJournal.getEntries();

    ASSERT_EQ(entries.size(), 2);
    EXPECT_EQ(entries[0].tick, 0);
    EXPECT_EQ(entries[0].description, "Поступил заказ");
    EXPECT_EQ(entries[1].tick, 5);
    EXPECT_EQ(entries[1].description, "Заказ выполнен");
    std::remove(filename.c_str());
}