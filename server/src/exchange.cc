#include "exchange.h"

#include <algorithm>
#include <chrono>

auto Exchange::Registration() -> common::uid_t {
  const auto uid = GenerateUID();
  m_accounts[uid] = Account();
  return uid;
}

auto Exchange::Login(common::uid_t uid) -> bool {
  return m_accounts.contains(uid);
}

auto Exchange::Order(const order::Data&& data) -> void {
  if (data.type == order::Type::kBuy) {
    m_orders_buy[data.instrument.code].emplace(std::forward<const order::Data>(data));
  } else if (data.type == order::Type::kSell) {
    m_orders_sell[data.instrument.code].emplace(std::forward<const order::Data>(data));
  }
  MatchOrders(data.instrument.code);
}

auto Exchange::Balance(common::uid_t uid) -> const std::unordered_map<order::instrument_t, double>& {
  return m_accounts[uid].m_instruments;
}

auto Exchange::GenerateUID() -> common::uid_t {
  return uid++;
}

auto Exchange::MatchOrders(order::instrument_t instrument) -> void {
  auto& b_queue = m_orders_buy[instrument];
  auto& s_queue = m_orders_sell[instrument];
  while (!b_queue.empty() && !s_queue.empty()) {
    order::Data b_data = b_queue.top();
    order::Data s_data = s_queue.top();

    const auto timestamp = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    s_data.timestamp = timestamp;
    b_data.timestamp = timestamp;
    
    if (b_data.price >= s_data.price) {
      Account& b_account = m_accounts[b_data.UID];
      Account& s_account = m_accounts[s_data.UID];

      double& b_amount = b_data.amount;
      double& s_amount = s_data.amount;
      
      if (b_amount > s_amount) {
        s_account.m_instruments[order::Instrument::RUB()] += s_amount * b_data.price;
        b_account.m_instruments[order::Instrument::RUB()] -= s_amount * b_data.price;
        s_account.m_instruments[b_data.instrument.code] -= s_amount;
        b_account.m_instruments[b_data.instrument.code] += s_amount;

        s_data.command(s_data);

        s_data.UID = b_data.UID;
        s_data.type = order::Type::kBuy;
        b_data.command(s_data);

        s_queue.pop();
        b_queue.pop();
        b_data.amount -= s_amount;
        b_queue.push(b_data);
      } else if (b_amount < s_amount) {
        s_account.m_instruments[order::Instrument::RUB()] += b_amount * b_data.price;
        b_account.m_instruments[order::Instrument::RUB()] -= b_amount * b_data.price;
        s_account.m_instruments[b_data.instrument.code] -= b_amount;
        b_account.m_instruments[b_data.instrument.code] += b_amount;

        b_data.command(b_data);

        b_data.UID = s_data.UID;
        b_data.type = order::Type::kSell;
        s_data.command(b_data);

        b_queue.pop();
        s_queue.pop();
        s_data.amount -= b_amount;
        s_queue.push(s_data);
      } else {
        s_account.m_instruments[order::Instrument::RUB()] += b_amount * b_data.price;
        b_account.m_instruments[order::Instrument::RUB()] -= s_amount * b_data.price;
        s_account.m_instruments[b_data.instrument.code] -= b_amount;
        b_account.m_instruments[b_data.instrument.code] += s_amount;

        b_data.command(b_data);
        s_data.command(s_data);

        b_queue.pop();
        s_queue.pop();
      }
    } else {
      break;
    }
  }
}