#pragma once

#include <array>

#include "order.h"

namespace balans {
  struct Data {
    order::instrument_t instrument;
    double amount;
  };

  struct Request {};

  struct Response {
    bool status;
    std::array<Data, order::Instrument::Size()> balance;
  };
}  // namespace balans