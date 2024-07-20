#pragma once

#include "common.h"

namespace registration {
  struct Request {};

  struct Response {
    bool status;
    common::uid_t uid;
  };
}  // namespace registration

