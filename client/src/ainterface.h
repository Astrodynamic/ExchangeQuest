#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

class AbstractInterface {
 public:
  AbstractInterface() = default;
  explicit AbstractInterface(const AbstractInterface &other) = delete;
  explicit AbstractInterface(AbstractInterface &&other) = delete;
  AbstractInterface &operator=(const AbstractInterface &other) = delete;
  AbstractInterface &operator=(AbstractInterface &&other) = delete;
  virtual ~AbstractInterface() = default;

  virtual auto Exec() -> void = 0 ;

 protected:
  auto RunMenu(const std::vector<std::function<bool(void)>> &func, std::size_t menu) -> bool;
  auto ShowMenu(const std::string &menu, const std::size_t items = 0U) -> std::size_t;

  [[nodiscard]] auto CheckInputItem(const std::int64_t min, const std::int64_t max) -> std::int64_t;
  [[nodiscard]] auto Exit() -> bool;

  static const std::vector<std::string> m_menus;
  std::vector<std::vector<std::function<bool(void)>>> m_funcs;
};