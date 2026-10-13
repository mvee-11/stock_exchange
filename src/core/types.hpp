#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace exchange::core {

using Price = std::int64_t;
using quantity = std::uint32_t;
using orderid = std::uint64_t;
using sequencenumber = std::uint64_t;
using timestamp = std::uint64_t;
using matchid = std::uint64_t;
using participantid = std::uint32_t;

inline constexpr Price PRICE_SCALE = 10000;

enum class Side : std::uint8_t {
  BUY,
  SELL,
};

enum class OrderType : std::uint8_t {
  LIMIT,
  MARKET,
  IOC,
  FOK,
  GTC,
  STOP,
  STOP_LIMIT,
  ICEBERG,
  POST_ONLY,
};

enum class OrderStatus : std::uint8_t {
  NEW,
  ACCEPTED,
  FILLED,
  PARTIALLY_FILLED,
  CANCELLED,
};

using Symbol = std::array<char, 8>;

constexpr Symbol make_symbol(std::string_view text) noexcept {
  Symbol symbol{};
  const auto length = std::min(symbol.size(), text.size());
  for (std::size_t index = 0; index < length; ++index) {
    symbol[index] = text[index];
  }
  return symbol;
}

constexpr std::string_view symbol_view(const Symbol &symbol) noexcept {
  std::size_t length = 0;
  while (length < symbol.size() && symbol[length] != '\0') {
    length++;
  }
  return std::string_view(symbol.data(), length);
}

struct SymbolLess {
  constexpr bool operator()(const Symbol &lhs,
                            const Symbol &rhs) const noexcept {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
  }
};

// below, we are saying - we are telling the complier that the variables are trivially copyable ie that we can directly copy the raw bites instead of having the typical 
// overhead that is associated. if this is wrong, gcc will throw an error ( that is what it means when we say static assert , also, this is done at compile time )
static_assert(std::is_trivially_copyable_v<Price>);
static_assert(std::is_trivially_copyable_v<quantity>);
static_assert(std::is_trivially_copyable_v<orderid>);
static_assert(std::is_trivially_copyable_v<sequencenumber>);
static_assert(std::is_trivially_copyable_v<timestamp>);
static_assert(std::is_trivially_copyable_v<matchid>);
static_assert(std::is_trivially_copyable_v<participantid>);

} // namespace exchange::core
