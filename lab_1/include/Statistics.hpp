#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <deque>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Order.hpp"

struct TickStatistics {
    std::int64_t tick;
    std::size_t waitingOrders;
    double onTimeProcent;
    double cookUtilization;
    double workplaceUtilization;
};


class Statistics {
    private:
    std::vector<TickStatistics> statistics;
    std::size_t completedOrders = 0;
    std::size_t completedOnTime = 0;

    public:
    void recordTick(std::int64_t tick, const std::deque<Order>& orders, double cookUti, double workplaceUti) {
        std::size_t waiting = 0;

        for (const Order& order : orders) {
            if (order.getStatus() == OrderStatus::Waiting) {
                ++waiting;
            }
        }

        double onTimeProcent = 0.0;
        if (completedOrders > 0) {
            onTimeProcent = static_cast<double>(completedOnTime) / completedOrders * 100.0;
        }

        statistics.push_back({tick, waiting, onTimeProcent, cookUti, workplaceUti});
    }

    const std::vector<TickStatistics>& getStatisticsList() const {
        return statistics;
    }
    void recordCompletedOrder(bool onTime) {
        ++completedOrders;

        if (onTime) {
            ++completedOnTime;
        }
    }
    void saveToCsv(const std::string& filename) const {
        std::ofstream file(filename);

        if (!file) {
            throw std::runtime_error("Не удалось открыть CSV-файл");
        }

        file << "tick, waitingOrders, onTimeProcent, cookUtilization, workplaceUtilization\n";

        for (const TickStatistics& stat : statistics) {
            file << stat.tick << ','
                << stat.waitingOrders << ','
                << stat.onTimeProcent << ','
                << stat.cookUtilization << ','
                << stat.workplaceUtilization << '\n';
        }
        if (!file) {
            throw std::runtime_error("Ошибка записи статистики");
        }
    }
};