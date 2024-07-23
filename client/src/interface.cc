#include "interface.h"

#include <iostream>
#include <regex>

#include "common.h"

const std::vector<std::string> Interface::AbstractInterface::m_menus{
    " -------------------------------------------------------------- \n"
    "|                         Exchange 1.0                         |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. Connect to the server                                     |\n"
    "| 2. Registration                                              |\n"
    "| 3. Login                                                     |\n"
    "| 4. Order                                                     |\n"
    "| 5. Balance                                                   |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "| Enter UID:                                                   |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. Create a new trading order                                |\n"
    "|    Format: [INSTRUMENT]|[SIDE]:[VOLUME]@[PRICE]              |\n"
    "|    INSTRUMENT: Trading instrument (e.g., USD)                |\n"
    "|    SIDE: BUY (for buying) or SELL (for selling)              |\n"
    "|    VOLUME: Number of units to trade                          |\n"
    "|    PRICE: Price per unit                                     |\n"
    "|                                                              |\n"
    "|    Example: USD|BUY:100@150.25                               |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                 No connection to the server                  |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                     Log in to the system                     |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                        Invalid Order                         |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|            Successful completion of the programme            |\n"
    " -------------------------------------------------------------- \n"};

Interface::Interface(Client &client) : m_client{client} {
  InitFuncMenus();
  ShowMenu(m_menus[MenuItem::kIntroduction]);
}

Interface::~Interface() {
  ShowMenu(m_menus[MenuItem::kCompletion]);
}

auto Interface::Exec() -> void {
  RunMenu(m_funcs[MenuFuncs::kMainFuncMenu], MenuItem::kMainMenu);
}

auto Interface::InitFuncMenus() -> void {
  m_funcs.resize(MenuFuncs::kMenuFuncsAll);

  m_funcs[MenuFuncs::kMainFuncMenu] = {
    std::bind(&Interface::Exit, this),
    [this]() -> bool {
      m_client.AsyncConnect(common::kIP.data(), std::to_string(common::kPort));
      return true;
    },
    [this]() -> bool {
      if (Validate()) {
        command::Data data;
        data.type = command::Type::kRegistrationRequest;
        m_client.Send(data);
      }
      return true;
    },
    [this]() -> bool {
      if (Validate()) {
        command::Data data;
        data.type = command::Type::kLoginRequest;

        ShowMenu(m_menus[MenuItem::kUIDMenu]);
        std::cin >> data.data.login_request.UID;
        std::cin.ignore();
        
        m_client.Send(data);
      }
      return true;
    },
    std::bind(&Interface::RunMenu, this, std::ref(m_funcs[MenuFuncs::kOrderFuncMenu]), MenuItem::kOrderMenu),
    [this]() -> bool {
      if (Validate()) {
        command::Data data;
        data.type = command::Type::kBalansRequest;
        m_client.Send(data);
      }
      return true;
    }
  };

  m_funcs[MenuFuncs::kOrderFuncMenu] = {
    std::bind(&Interface::Exit, this),
    [this]() -> bool {
      if (Validate(true)) {
        std::string input;
        std::cin >> input;
        std::cin.ignore();

        static std::regex regex(R"(^([A-Z]+)\|(BUY|SELL):([1-9]\d*(\.\d+)?)@([1-9]\d*(\.\d+)?)$)");
        std::smatch match;
        if (std::regex_match(input, match, regex)) {
          command::Data request;
          request.type = command::Type::kOrderRequest;

          auto& order_request = request.data.order_request;
          if (std::string instrument = match[1]; instrument == "USD") {
            order_request.instrument.Set(order::Instrument::Type::kCurrency, static_cast<std::uint8_t>(order::Instrument::Currency::kUSD));
          } else {
            ShowMenu(m_menus[MenuItem::kOrderErrorMenu]);
            return true;
          }

          order_request.amount = std::stod(match[3]);
          order_request.price = std::stod(match[5]);

          std::string side = match[2];
          if (side == "BUY") {
            order_request.type = order::Type::kBuy;
          } else if (side == "SELL") {
            order_request.type = order::Type::kSell;
          }
          
          m_client.Send(request);
        } else {
          ShowMenu(m_menus[MenuItem::kOrderErrorMenu]);
        }
      }
      return true;
    }
  };
}

auto Interface::Validate(bool loged) -> bool {
  bool flag{true};
  if (flag = m_client.Connected(); !flag) {
    ShowMenu(m_menus[MenuItem::kNoConnectMenu]);
  } else if (flag = m_client.Logined() || !loged; !flag) {
    ShowMenu(m_menus[MenuItem::kLoginMenu]);
  }
  return flag;
}
