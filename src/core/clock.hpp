#pragma once

#include <chrono>

#include "core/types.hpp"

namespace exchange::core {

class LogicalClock {
  public:
    [[nodiscard]] timestamp next() noexcept { return ++current_; }

    [[nodiscard]] timestamp current() noexcept { return current_; }

    void reset() noexcept { current_ = 0; }

  private:
    timestamp current_{0};
};

[[nodiscard]] inline timestamp now_ns() noexcept { // now_ns ( nanoseconds ) is easier to call but more appropriate name would be steady timestamp ( not the wall clock but system clock )
    const auto now = std::chrono::steady_clock::now().time_since_epoch() ;

    return static_cast<timestamp>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(now).count()
    ); 
}
} // namespace exchange::core
