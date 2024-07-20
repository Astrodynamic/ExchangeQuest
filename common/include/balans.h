#pragma once

#include "common.h"

namespace balans {
  struct Request {
    common::uid_t uid;
  };

  struct Response {
    bool status;
    double balance;
  };
}  // namespace balans