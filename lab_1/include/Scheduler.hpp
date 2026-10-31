#pragma once

#include "Order.hpp"
#include <deque>
#include <algorithm>
#include <vector>

enum class SchedulingPolicy {
    FIFO,
    EDF
};


class Scheduler {
    private:
    SchedulingPolicy policy = SchedulingPolicy::FIFO;

    public:
    void setPolicy(SchedulingPolicy new_policy) {
        policy = new_policy;
    }

    std::vector<std::size_t> getOrderIndeces(const std::deque<Order>& orders) const {
        std::vector<std::size_t> indeces;
        for (std::size_t i = 0; i< orders.size(); i++) {
            indeces.push_back(i);
        }
        if (policy == SchedulingPolicy::EDF) {
            std::stable_sort(indeces.begin(), indeces.end(), [&orders](std::size_t a, std::size_t b) {
                return orders[a].getDeadline() < orders[b].getDeadline();});
        }
        return indeces;

    }
};