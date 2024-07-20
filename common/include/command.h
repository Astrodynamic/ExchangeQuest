#pragma once

#include "common.h"
#include "login.h"
#include "registration.h"
#include "order.h"
#include "balans.h"
#include "instruments.h"

namespace command {
enum class Type : std::uint8_t {
  kRegistrationRequest,
  kRegistrationResponse,
  kLoginRequest,
  kLoginResponse,
  kOrderRequest,
  kOrderResponce,
  kBalansRequest,
  kBalansResponse,
  kInstrumentsRequest,
  kInstrumentsResponse
};

struct Data {
  common::uid_t UID;
  Type type;
  union {
    registration::Request registration_request;
    registration::Response registration_response;
    login::Request login_request;
    login::Response login_response;
    order::Request order_request;
    order::Response order_response;
    balans::Request balans_request;
    balans::Response balans_response;
    instruments::Request instruments_request;
    instruments::Response instruments_response;
  } data;
  std::uint64_t timestamp;
};
}  // namespace command