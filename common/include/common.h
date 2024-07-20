#pragma once

#include <cstdint>
#include <string_view>

namespace common {

using uid_t = std::uint64_t;

constexpr std::string_view kIP = "127.0.0.1";
constexpr static std::uint16_t kPort = 15001;

}  // namespace common