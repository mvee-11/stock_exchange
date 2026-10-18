#pragma once

#include "core/types.hpp"
#include <cstddef>
#include <type_traits>

namespace exchange::matching {

struct Level ;

struct alignas(64) Order {
    core::orderid id{0};
    core::Side side{core::Side::BUY};
    core::Price price{0};  // the ones in curly braces are initial / default val
    core::quantity qty{0}; // the qty of the order still remaining
    core::quantity original_qty{0};
    core::quantity hidden_qty{0};
    core::quantity display_qty{0};
    core::timestamp Timestamp{0};
    core::OrderType type{core::OrderType::LIMIT};
    core::OrderStatus status{core::OrderStatus::NEW};
    core::participantid participant_id{0};
    core::Price trigger_price{0};
    core::quantity peak_qty{0};
    Order *prev{nullptr}; // this is called intrusive list . pointers are
                          // directly part of the data type
    Order *next{nullptr}; // this makes it O(1) time for adding and removing
                          // from anywhere in the list
    Level *parent_level{nullptr};

    [[nodiscard]] bool is_stop_order() const noexcept {
        return type == core::OrderType::STOP || type == core::OrderType::STOP_LIMIT ;
    } 

};

static_assert(sizeof(Order) <= 128, "order should be compact enough for cache-line");
static_assert(std::is_standard_layout_v<Order>);
} // namespace exchange::matching
