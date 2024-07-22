#pragma once

#include "ainterface.h"
#include "client.h"

class Interface final : virtual public AbstractInterface {
 public:
  Interface(Client &client);
  explicit Interface(const Interface &other) = delete;
  explicit Interface(Interface &&other) = delete;
  Interface &operator=(const Interface &other) = delete;
  Interface &operator=(Interface &&other) = delete;
  ~Interface();

  virtual auto Exec() -> void final;

 private:
  enum MenuFuncs : std::size_t {
    kMainFuncMenu = 0U,
    kOrderFuncMenu,
    kMenuFuncsAll
  };

  enum MenuItem : std::size_t {
    kIntroduction = 0U,
    kMainMenu,
    kUIDMenu,
    kOrderMenu,
    kNoConnectMenu,
    kLoginMenu,
    kOrderErrorMenu,
    kCompletion
  };

  Client& m_client;

  auto InitFuncMenus() -> void;
  auto Validate(bool loged = false) -> bool;
};