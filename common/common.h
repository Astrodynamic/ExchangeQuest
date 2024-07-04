#pragma once

#include <string>

struct Order {
  int client_id;
  double amount;
  double price;
  std::string side;

  bool operator<(const Order& other) const {
    return price < other.price;
  }
};
