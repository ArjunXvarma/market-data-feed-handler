//
// Created by Arjun Varma on 30/08/25.
//

#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <vector>
#include <atomic>
#include <cstddef>

template <typename T>
class LockFreeRingBuffer {
private:
    size_t increment(size_t idx) const {
        return (idx + 1) % capacity_;
    }

    std::vector<T> buffer_;
    const size_t capacity_;

    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;

public:
    explicit LockFreeRingBuffer(size_t capacity)
        : buffer_(capacity), capacity_(capacity), head_(0), tail_(0) {}

    bool push(const T& item) {
        auto head = head_.load(std::memory_order_relaxed);
        auto next_head = increment(head);

        if (next_head == tail_.load(std::memory_order_acquire)) {
            return false; // buffer full
        }

        buffer_[head] = item;
        head_.store(next_head, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        auto tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire)) {
            return false;
        }

        item = buffer_[tail];
        tail_.store(increment(tail), std::memory_order_release);
        return true;
    }

    bool empty() const {
        return head_.load(std::memory_order_acquire) ==
               tail_.load(std::memory_order_acquire);
    }

    bool full() const {
        auto next_head = increment(head_.load(std::memory_order_acquire));
        return next_head == tail_.load(std::memory_order_acquire);
    }

    size_t size() const {
        auto head = head_.load(std::memory_order_acquire);
        auto tail = tail_.load(std::memory_order_acquire);
        if (head >= tail) return head - tail;
        return capacity_ - (tail - head);
    }

    size_t capacity() const { return capacity_; }
};

#endif //RINGBUFFER_HPP
