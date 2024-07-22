#pragma once

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>

#include "common.h"

namespace order {
  
union Instrument {
  enum class Type: std::uint8_t {
    kCurrency = 0,
    kAll
  };

  enum class Currency: std::uint8_t {
    kRUB = 0,
    kUSD,
    kAll
  };

  struct {
    Type type;
    std::uint8_t id;
  } data;
  std::uint16_t code;

  constexpr static auto Code(Type type, std::uint8_t id) -> std::uint16_t {
    return (static_cast<std::uint16_t>(type) << 8) | id;
  }

  constexpr static auto RUB() -> std::uint16_t {
    return Code(Type::kCurrency, static_cast<std::uint8_t>(Currency::kRUB));
  }

  constexpr static auto USD() -> std::uint16_t {
    return Code(Type::kCurrency, static_cast<std::uint8_t>(Currency::kUSD));
  }

  constexpr static auto Size() -> std::size_t {
    return static_cast<std::size_t>(Currency::kAll);
  }

  void Set(Type type, std::uint8_t id) {
    data.type = type;
    data.id = id;
    code = Code(type, id);
  }
};

using instrument_t = decltype(Instrument::code);

enum class Type : std::uint8_t {
  kBuy = 0,
  kSell,
  kCount
};

struct Data {
  common::uid_t UID;
  Instrument instrument;
  double amount;
  double price;
  Type type;
  std::uint64_t timestamp;
  std::function<void(Data)> command;

  auto operator<(const Data& other) const -> bool {
    if (price < other.price) {
      return true;
    } else if (std::abs(price - other.price) < 1.0e-6) {
      return timestamp > other.timestamp;
    }
    return false;
  }

  auto operator>(const Data& other) const -> bool {
    if (price > other.price) {
      return true;
    } else if (std::abs(price - other.price) < 1.0e-6) {
      return timestamp > other.timestamp;
    }
    return false;
  }
};

struct Request {
  Instrument instrument;
  double amount;
  double price;
  Type type;
};

struct Response {
  Instrument instrument;
  double amount;
  double price;
  Type type;
};

}  // namespace order