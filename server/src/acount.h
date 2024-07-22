#pragma once

#include <unordered_map>

#include <order.h>

struct Account {
  std::unordered_map<order::instrument_t, double> m_instruments {
    {order::Instrument::RUB(), 0.0 },
    {order::Instrument::USD(), 0.0 }
  };
};