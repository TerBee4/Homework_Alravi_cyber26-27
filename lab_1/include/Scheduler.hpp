#pragma once

#include "Order.hpp"
#include <deque>
#include <algorithm>
#include <vector>
#include <stdexcept>

enum class SchedulingPolicy {
    FIFO,
    EDF
};


class ScheduleStrategy {
public:
    virtual ~ScheduleStrategy() = default;
    virtual std::vector<std::size_t> getOrderIndeces(const std::deque<Order>& orders) const = 0;
};


class FIFOStrategy : public ScheduleStrategy {
public:
    std::vector<std::size_t> getOrderIndeces(const std::deque<Order>& orders) const override {
        std::vector<std::size_t> indices;
        for (std::size_t i = 0; i < orders.size(); ++i) {
            indices.push_back(i);
        }
        return indices;
    }
};


class EDFStrategy : public ScheduleStrategy {
public:
    std::vector<std::size_t> getOrderIndeces(const std::deque<Order>& orders) const override {
        std::vector<std::size_t> indices;
        for (std::size_t i = 0; i < orders.size(); ++i) {
            indices.push_back(i);
        }
        std::stable_sort(indices.begin(), indices.end(), [&orders](std::size_t a, std::size_t b) {
        return orders[a].getDeadline() < orders[b].getDeadline();});
        return indices;
    }
};


class ScheduleStrategyFactory {
public:
    static const ScheduleStrategy& create(SchedulingPolicy policy) {
        static const FIFOStrategy fifo;
        static const EDFStrategy edf;

        switch (policy) {
            case SchedulingPolicy::FIFO:
                return fifo;

            case SchedulingPolicy::EDF:
                return edf;
        }
        throw std::invalid_argument("Неизвестная политика");
    }
};


class Scheduler {
    private:
    SchedulingPolicy policy = SchedulingPolicy::FIFO;

    public:
    void setPolicy(SchedulingPolicy new_policy) {
        ScheduleStrategyFactory::create(new_policy);
        policy = new_policy;
    }

    SchedulingPolicy getPolicy() const {
        return policy;
    }

    std::vector<std::size_t> getOrderIndeces(const std::deque<Order>& orders) const {
        const ScheduleStrategy& strategy = ScheduleStrategyFactory::create(policy);
        return strategy.getOrderIndeces(orders);
    }
};


