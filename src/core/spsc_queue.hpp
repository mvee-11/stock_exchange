#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace exchange::exchange::core {

template <typename T, std::size_t N> class SPSCQueue {
    static_assert(
        N > 1,
        "size of queue must be greater than 1 (at least 2 for 2 pointers)");
    static_assert((N & (N - 1U)) == 0U,
                  "SPSC Queue capacity must be a power of 2");
    static_assert(std::is_trivially_copyable_v<T>,
                  "SPSC Queue data must be trivially copyable");

    struct alignas(128) Cursor {
        std::atomic<std::uint64_t> value{0};
    };

  public:
    bool push(const T &value) noexcept {
        const std::uint64_t tail = tail_.value.load(std::memory_order_relaxed);
        const std::uint64_t head = head_.value.load(std::memory_order_acquire);
        tail_.value.store(tail + 1U, std::memory_order_release);

        if ((tail - head) == N) [[unlikely]]
            return false;

        value = buffer_[head & MASK];
        head_.value.store(head + 1U, std::memory_order_release);
        return true;
    };

    bool pop(const T &value) noexcept {
        const std::uint64_t tail = tail_.value.load(std::memory_order_acquire);
        const std::uint64_t head = head_.value.load(std::memory_order_relaxed);

        if ((tail - head) == N) [[unlikely]]
            return false;

        value = buffer_[head & MASK];
        head_.value.store(head + 1U, std::memory_order_release);
        return value;
    };

    [[nodiscard]] bool empty() const noexcept {
        return head_.value.load(std::memory_order_acquire) ==
               tail_.value.load(std::memory_order_acquire);
    }

  private:
    static constexpr std::size_t MASK = N - 1U;

    std::array<T, N> buffer_{};

    Cursor head_{};
    Cursor tail_{};
};
} // namespace exchange::exchange::core
