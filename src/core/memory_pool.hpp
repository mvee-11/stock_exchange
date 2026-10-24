#pragma once

#include <array>
#include <cstddef>
#include <memory>

// what this file does is it gives out 'n' buckets of memory for the program to
// store objects in instead of calling the heap which increases the latency in
// the hot path . so instead of it being spread out in memory, it is a
// contiguous slot of memory such that objects can be created and use this
// memory and then free the same memory when it is destroyed so that the next
// object can use it . it is PRE-ALLOCATED
// doesn't use the 'new' keyword and is O(1) time
//

namespace exchange::core {

template <typename T, std::size_t N>

class MemoryPool {
    static_assert(N > 0, "mempool capacity must be +ve");
    static_assert(sizeof(T) >= sizeof(void *),
                  "object storage must fit in intrusive free-list pointer");

    struct alignas((alignof(T) > 64U) ? alignof(T) : 64U) Slot {
        std::array<std::byte, sizeof(T)> storage{};
    };

    struct FreeNode {
        FreeNode *next;
    };

  public:
    MemoryPool() : slots_(std::make_unique<Slot[]>(N)) {
        initialize_free_list();
    }

    T *allocate() noexcept {
        if (free_list_head_ == nullptr) [[unlikely]] {
            return nullptr;
        }

        FreeNode *node = free_list_head_;
        free_list_head_ = free_list_head_->next;
        allocated_count++;

        return reinterpret_cast<T *>(node);
    }

    void deallocate(T *object) noexcept {
        if (object == nullptr) [[unlikely]] {
            return;
        }
        std::destroy_at(object);
        auto *node = reinterpret_cast<FreeNode *>(object);
        node->next = free_list_head_;
        free_list_head_ = node;
        allocated_count--;
    }

  private:
    void initialize_free_list() noexcept {
        free_list_head_ = nullptr;
        for (std::size_t index = 0; index < N; index++) {
            auto *node =
                reinterpret_cast<FreeNode *>(slots_[index].storage.data());
            node->next = free_list_head_;
            free_list_head_ = node;
        }
    }

    std::unique_ptr<Slot[]> slots_;

    FreeNode *free_list_head_{nullptr};

    std::size_t allocated_count{0};
};
} // namespace exchange::core
