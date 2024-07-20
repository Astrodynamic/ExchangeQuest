#pragma once

#include <cstdint>
#include <cstdlib>
#include <functional>

#include "common.h"

namespace order {
  
union Instrument {
  enum class Type: std::uint8_t {
    kCurrency
  };

  enum class Currency: std::uint8_t {
    kRUB,
    kUSD
  };

  struct {
    Type type;
    std::uint8_t id;
  } data;
  std::uint16_t code;

  static std::uint16_t Code(Type type, std::uint8_t id) {
    return (static_cast<std::uint16_t>(type) << 8) | id;
  }

  static std::uint16_t RUB() {
    return Code(Type::kCurrency, static_cast<std::uint8_t>(Currency::kRUB));
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

  bool operator==(const Amount& other) const {
    return !(*this < other) && !(*this > other);
  }

  bool operator<(const Amount& other) const {
    if (type == Type::kInteger) {
      return data.integer < other.data.integer;
    } else if (type == Type::kDecimal) {
      return data.decimal < other.data.decimal;
    }
    return false;
  }

  bool operator<=(const Amount& other) const {
    return *this < other || *this == other;
  }

  bool operator>(const Amount& other) const {
    if (type == Type::kInteger) {
      return data.integer > other.data.integer;
    } else if (type == Type::kDecimal) {
      return data.decimal > other.data.decimal;
    }
    return false;
  }

  bool operator>=(const Amount& other) const {
    return *this > other || *this == other;
  }

  Amount& operator+=(const Amount& other) {
    if (type == Type::kInteger) {
      data.integer += other.data.integer;
    } else if (type == Type::kDecimal) {
      data.decimal += other.data.decimal;
    }
    return *this;
  }

  Amount& operator-=(const Amount& other) {
    if (type == Type::kInteger) {
      data.integer -= other.data.integer;
    } else if (type == Type::kDecimal) {
      data.decimal -= other.data.decimal;
    }
    return *this;
  }

  Amount operator*(double other) const {
    Amount result = *this;
    if (type == Type::kInteger) {
      result.data.integer *= other;
    } else if (type == Type::kDecimal) {
      result.data.decimal *= other;
    }
    return result;
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

  bool operator<(const Data& other) const {
    if (price < other.price) {
      return true;
    } else if (std::abs(price - other.price) < 1.0e-6) {
      return timestamp > other.timestamp;
    }
    return false;
  }

  bool operator>(const Data& other) const {
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