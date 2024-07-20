#pragma once

#include <unordered_map>

#include "order.h"

namespace instruments {
  struct Request {
    common::uid_t uid;
  };

  struct Response {
    // std::unordered_map<order::instrument_t, order::Amount> instruments;

    // Response() = default;
  };
}  // namespace instruments