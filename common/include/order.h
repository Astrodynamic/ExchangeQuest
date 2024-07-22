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

struct Amount {
  enum class Type: std::uint8_t {
    kInteger,
    kDecimal
  } type;

  union {
    std::uint64_t integer;
    double decimal;
  } data;

  auto operator==(const Amount& other) const -> bool {
    return !(*this < other) && !(*this > other);
  }

  auto operator<(const Amount& other) const -> bool {
    if (type == Type::kInteger) {
      return data.integer < other.data.integer;
    } else if (type == Type::kDecimal) {
      return data.decimal < other.data.decimal;
    }
    return false;
  }

  auto operator<=(const Amount& other) const -> bool {
    return *this < other || *this == other;
  }

  auto operator>(const Amount& other) const -> bool {
    if (type == Type::kInteger) {
      return data.integer > other.data.integer;
    } else if (type == Type::kDecimal) {
      return data.decimal > other.data.decimal;
    }
    return false;
  }

  auto operator>=(const Amount& other) const -> bool {
    return *this > other || *this == other;
  }

  auto operator+=(const Amount& other) -> Amount& {
    if (type == Type::kInteger) {
      data.integer += other.data.integer;
    } else if (type == Type::kDecimal) {
      data.decimal += other.data.decimal;
    }
    return *this;
  }

  auto operator-=(const Amount& other) -> Amount& {
    if (type == Type::kInteger) {
      data.integer -= other.data.integer;
    } else if (type == Type::kDecimal) {
      data.decimal -= other.data.decimal;
    }
    return *this;
  }

  auto operator*(double other) const -> Amount {
    Amount result = *this;
    if (type == Type::kInteger) {
      result.data.integer *= other;
    } else if (type == Type::kDecimal) {
      result.data.decimal *= other;
    }
    return result;
  }

  friend std::ostream& operator<<(std::ostream& os, const Amount& amount) {
    if (amount.type == Amount::Type::kInteger) {
      os << amount.data.integer;
    } else if (amount.type == Amount::Type::kDecimal) {
      os << amount.data.decimal;
    }
    return os;
  }
};

enum class Type : std::uint8_t {
  kBuy = 0,
  kSell,
  kCount
};

struct Data {
  common::uid_t UID;
  Instrument instrument;
  Amount amount;
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
  Amount amount;
  double price;
  Type type;
};

struct Response {
  Instrument instrument;
  Amount amount;
  double price;
  Type type;
};

}  // namespace order