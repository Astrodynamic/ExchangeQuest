#include "interface.h"

#include <thread>

#include "common.h"

const std::vector<std::string> Interface::AbstractInterface::m_menus{
    " -------------------------------------------------------------- \n"
    "|                          Enigma 1.0                          |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. Connection to the server                                  |\n"
    "| 2. Registration                                              |\n"
    "| 3. Login                                                     |\n"
    "| 4. Information decoding                                      |\n"
    "| 5. Genarate the enigma configuration setting                 |\n"
    "| 5. Genarate the enigma configuration setting                 |\n"
    "| 5. Genarate the enigma configuration setting                 |\n"
    "| 5. Genarate the enigma configuration setting                 |\n"
    "| 5. Genarate the enigma configuration setting                 |\n"
    "| 5. Genarate the enigma configuration setting                 |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. Enter the full path to the file ...                       |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. Enter number of rotors (1 - 255)                          |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|             A file with that name does not exist             |\n"
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
      command::Data data;
      data.type = command::Type::kRegistrationRequest;
      m_client.Send(data);
      return true;
    },
    [this]() -> bool {
      command::Data data;
      data.type = command::Type::kLoginRequest;
      m_client.Send(data);
      return true;
    },
    std::bind(&Interface::RunMenu, this, std::ref(m_funcs[MenuFuncs::kDecode]), MenuItem::kLoadMenu),
    std::bind(&Interface::RunMenu, this, std::ref(m_funcs[MenuFuncs::kGenerate]), MenuItem::kGenerateMenu)
  };

  m_funcs[MenuFuncs::kLoadConf] = {
      std::bind(&Interface::Exit, this),
      [this]() -> bool {
        m_client.AsyncConnect(common::kIP.data(), std::to_string(common::kPort));
        return true;
      }};

  m_funcs[MenuFuncs::kUnloadConf] = {
      std::bind(&Interface::Exit, this),
      [this]() -> bool {
        command::Data data;
        data.type = command::Type::kRegistrationRequest;
        m_client.Send(data);
        return true;
      }};

  m_funcs[MenuFuncs::kEncode] = {
      std::bind(&Interface::Exit, this),
      [this]() -> bool {
        return true;
      }};

  m_funcs[MenuFuncs::kDecode] = {
      std::bind(&Interface::Exit, this),
      [this]() -> bool {
        return true;
      }
  };

  m_funcs[MenuFuncs::kGenerate] = {
    std::bind(&Interface::Exit, this),
    [this]() -> bool {
      return true;
    }
  };
}
