#pragma once

#include "common.h"

namespace login {
  struct Request {
    common::uid_t UID;
  };

  struct Response {
    bool status;
    common::uid_t UID;
  };
}  // namespace registration