#pragma once

#include "common.h"

namespace login {
  struct Request {
    common::uid_t uid;
  };

  struct Response {
    bool status;
  };
}  // namespace registration