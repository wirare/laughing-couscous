#pragma once

#include <algorithm>
#include <random>
#include <deque>
#include <NN.hpp>

class ReplayBuffer {
public:
    ReplayBuffer(size_t capacity) : capacity(capacity) {}

    void add(const Experience& exp) 
	{
        if (buffer.size() >= capacity)
            buffer.pop_front();
        buffer.push_back(exp);
    }

    std::vector<Experience> sample(size_t batch_size) 
	{
        std::vector<Experience> result;
        std::sample(buffer.begin(), buffer.end(), std::back_inserter(result),
                    batch_size, std::mt19937{std::random_device{}()});
        return result;
    }

    bool is_ready(size_t batch_size) const 
	{
        return buffer.size() >= batch_size;
    }

private:
    std::deque<Experience> buffer;
    size_t capacity;
};

