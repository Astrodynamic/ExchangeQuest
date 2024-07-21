#include "ainterface.h"

#include <iostream>

auto AbstractInterface::RunMenu(const std::vector<std::function<bool(void)>> &func, std::size_t menu) -> bool {
  bool flag{};
  std::size_t item{};
  std::size_t items{func.size()};
  do {
    item = ShowMenu(m_menus[menu], items);
  } while (items && (flag = func[item]()));

  return !flag;
}

auto AbstractInterface::ShowMenu(const std::string &menu, const std::size_t items) -> std::size_t {
  std::cout << menu;
  return items ? CheckInputItem(-1, items) : 0;
}

[[nodiscard]] auto AbstractInterface::CheckInputItem(const std::int64_t min, const std::int64_t max) -> std::int64_t {
  std::string line;
  std::getline(std::cin, line);

  std::int64_t result;
  while (!sscanf(line.c_str(), "%ld", &result) || result <= min || result >= max) {
    std::cout << "Incorrect input, try again: ";
    std::getline(std::cin, line);
  }

  return result;
}

[[nodiscard]] auto AbstractInterface::Exit() -> bool {
  return false;
}