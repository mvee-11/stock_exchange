#pragma once

#include "core/types.hpp"
#include <cstdint>
#include <type_traits>

namespace exchange::matching {

using core::matchid;
using core::orderid;
using core::Price;
using core::quantity;
using core::sequencenumber;
using core::Side;
using core::Symbol;
using core::timestamp;

enum class ReasonCode : std::uint16_t {
    NONE = 0,
    ZERO_QUANTITY,
    NEGATIVE_PRICE,
    BOOK_EMPTY,
    DUPLICATE_ORDER_ID,
    ORDER_NOT_FOUND,
    ORDER_NOT_ACTIVE,
    FOK_INSUFFICIENT_LIQUIDITY,
    POST_ONLY_WOULD_CROSS,
    INVALID_MODIFICATION,
    INVALIDID_ICEBERG_DISPLAY,
    ORDER_POOL_EXHAUSTED,
};

enum class EventType : std::uint8_t {
    ORDER_RESTED,
    ORDER_ACCEPTED,
    ORDER_REJECTED,
    ORDER_REDUCED,
    ORDER_PARTIALLY_FILLED,
    ORDER_FILLED,
    ORDER_CAsCELLED,
    TRADE,

};


struct OrderAccepted {
  sequencenumber sequence_number;
  timestamp timestamp;
  orderid order_id;
  Symbol symbol;
  Side side;
  Price price;
  quantity qty;
  core::OrderType order_type;
};

} // namespace exchange::matching
