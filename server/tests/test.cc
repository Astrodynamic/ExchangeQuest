#include <gtest/gtest.h>

#include "exchange.h"

#include <iostream>

TEST(Exchange, Registration) {
  Exchange exchange;
  ASSERT_EQ(exchange.Registration(), static_cast<common::uid_t>(common::UIDType::kClient) + 0);
  ASSERT_EQ(exchange.Registration(), static_cast<common::uid_t>(common::UIDType::kClient) + 1);
  ASSERT_EQ(exchange.Registration(), static_cast<common::uid_t>(common::UIDType::kClient) + 2);
}

TEST(Exchange, Login) {
  Exchange exchange;
  exchange.Registration();
  ASSERT_TRUE(exchange.Login(static_cast<common::uid_t>(common::UIDType::kClient) + 3));
  ASSERT_FALSE(exchange.Login(static_cast<common::uid_t>(common::UIDType::kClient) + 4));
  ASSERT_FALSE(exchange.Login(static_cast<common::uid_t>(common::UIDType::kCommon)));
  ASSERT_FALSE(exchange.Login(static_cast<common::uid_t>(common::UIDType::kServer)));
}

TEST(Exchange, Order_1) {
  Exchange exchange;

  order::Instrument i_usd;
  i_usd.Set(order::Instrument::Type::kCurrency, static_cast<std::uint8_t>(order::Instrument::Currency::kUSD));

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 0,
    .instrument = i_usd,
    .amount = 10.0,
    .price = 62.0,
    .type = order::Type::kBuy,
    .command = [](order::Data) {}
  });

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 1,
    .instrument = i_usd,
    .amount = 20.0,
    .price = 63.0,
    .type = order::Type::kBuy,
    .command = [](order::Data) {}
  });

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 2,
    .instrument = i_usd,
    .amount = 50.0,
    .price = 61.0,
    .type = order::Type::kSell,
    .command = [](order::Data) {}
  });

  auto balance_0 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::RUB()], -10 * 62.0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::USD()], +10.0);
  
  auto balance_1 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 1);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::RUB()], -20 * 63.0);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::USD()], +20.0);
  
  auto balance_2 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 2);
  ASSERT_FLOAT_EQ(balance_2[order::Instrument::RUB()], +10 * 62.0 + 20 * 63.0);
  ASSERT_FLOAT_EQ(balance_2[order::Instrument::USD()], -10.0 - 20.0);
}

TEST(Exchange, Order_2) {
  Exchange exchange;

  order::Instrument i_usd;
  i_usd.Set(order::Instrument::Type::kCurrency, static_cast<std::uint8_t>(order::Instrument::Currency::kUSD));

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 0,
    .instrument = i_usd,
    .amount = 1000.0,
    .price = 62.0,
    .type = order::Type::kSell,
    .command = [](order::Data) {}
  });

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 1,
    .instrument = i_usd,
    .amount = 20.0,
    .price = 63.0,
    .type = order::Type::kBuy,
    .command = [](order::Data) {}
  });

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 2,
    .instrument = i_usd,
    .amount = 50.0,
    .price = 61.0,
    .type = order::Type::kBuy,
    .command = [](order::Data) {}
  });

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 3,
    .instrument = i_usd,
    .amount = 1000.0,
    .price = 68.0,
    .type = order::Type::kBuy,
    .command = [](order::Data) {}
  });

  auto balance_0 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::RUB()], +20.0 * 63.0 + 980.0 * 68.0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::USD()], -20.0 - 980.0);
  
  auto balance_1 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 1);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::RUB()], -20.0 * 63.0);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::USD()], +20.0);
  
  auto balance_2 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 2);
  ASSERT_FLOAT_EQ(balance_2[order::Instrument::RUB()], 0.0);
  ASSERT_FLOAT_EQ(balance_2[order::Instrument::USD()], 0.0);
  
  auto balance_3 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 3);
  ASSERT_FLOAT_EQ(balance_3[order::Instrument::RUB()], -980.0 * 68.0);
  ASSERT_FLOAT_EQ(balance_3[order::Instrument::USD()], +980.0);
}

TEST(Exchange, Order_3) {
  Exchange exchange;

  order::Instrument i_usd;
  i_usd.Set(order::Instrument::Type::kCurrency, static_cast<std::uint8_t>(order::Instrument::Currency::kUSD));

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 0,
    .instrument = i_usd,
    .amount = 10.0,
    .price = 62.0,
    .type = order::Type::kSell,
    .command = [](order::Data) {}
  });

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 1,
    .instrument = i_usd,
    .amount = 20.0,
    .price = 61.0,
    .type = order::Type::kBuy,
    .command = [](order::Data) {}
  });

  auto balance_0 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::RUB()], 0.0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::USD()], 0.0);
  
  auto balance_1 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 1);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::RUB()], 0.0);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::USD()], 0.0);
}

TEST(Exchange, Balance) {
  Exchange exchange;
  auto balance_0 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::RUB()], 0.0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::USD()], 0.0);

  auto balance_1 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 1);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::RUB()], 0.0);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::USD()], 0.0);

  order::Instrument i_usd;
  i_usd.Set(order::Instrument::Type::kCurrency, static_cast<std::uint8_t>(order::Instrument::Currency::kUSD));

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 0,
    .instrument = i_usd,
    .amount = 10.0,
    .price = 62.0,
    .type = order::Type::kSell,
    .command = [](order::Data) {}
  });

  exchange.Order(order::Data {
    .UID = static_cast<common::uid_t>(common::UIDType::kClient) + 1,
    .instrument = i_usd,
    .amount = 20.0,
    .price = 63.0,
    .type = order::Type::kBuy,
    .command = [](order::Data) {}
  });

  balance_0 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::RUB()], +10.0 * 63.0);
  ASSERT_FLOAT_EQ(balance_0[order::Instrument::USD()], -10.0);

  balance_1 = exchange.Balance(static_cast<common::uid_t>(common::UIDType::kClient) + 1);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::RUB()], -10.0 * 63.0);
  ASSERT_FLOAT_EQ(balance_1[order::Instrument::USD()], +10.0);
}