#pragma once

#include <queue>
#include <atomic>
#include <unordered_map>

#include "common.h"
#include "acount.h"

class Exchange {
 public:
  auto Registration() -> common::uid_t;
  auto Login(common::uid_t uid) -> bool;
  auto Order(const order::Data&& data) -> void;
  auto Balance(common::uid_t uid) -> const std::unordered_map<order::instrument_t, double>&;

 private:
  inline static std::atomic<common::uid_t> uid{static_cast<common::uid_t>(common::UIDType::kClient)};
  std::unordered_map<common::uid_t, Account> m_accounts;
  std::unordered_map<order::instrument_t, std::priority_queue<order::Data, std::vector<order::Data>, std::greater<order::Data>>> m_orders_buy;
  std::unordered_map<order::instrument_t, std::priority_queue<order::Data, std::vector<order::Data>, std::less<order::Data>>> m_orders_sell;

  static auto GenerateUID() -> common::uid_t;
  auto MatchOrders(order::instrument_t instrument) -> void;
};
