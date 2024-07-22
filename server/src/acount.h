#pragma once

#include <unordered_map>

#include <order.h>

struct Account {
  std::unordered_map<order::instrument_t, order::Amount> m_instruments {
    {order::Instrument::RUB(), { .type = order::Amount::Type::kDecimal, .data = { .decimal = 0.0 } } },
    {order::Instrument::USD(), { .type = order::Amount::Type::kDecimal, .data = { .decimal = 0.0 } } }
  };
};