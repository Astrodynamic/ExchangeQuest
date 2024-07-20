# ExchangeQuest

- [ExchangeQuest](#exchangequest)
  - [Introduction](#introduction)
  - [Requirements](#requirements)
  - [Building and Running](#building-and-running)
  - [Testing](#testing)
  - [LICENSE](#license)

## Introduction
ExchangeQuest is a simple client-server trading exchange system that allows users to trade USD for RUB. This project demonstrates the implementation of a multi-client trading system with order matching logic using C++20. It leverages Boost.Asio for networking and GoogleTest for testing.

## Requirements
- Boost.Asio (latest stable version)
- CMake (version 3.21 or later)
- C++20 or later
- GoogleTest (for testing)

## Building and Running
To build the project, follow these steps:


```sh
make
```

To run the server:
```sh
./server/ExchangeQuestServer
```

To run the client:
```sh
./client/ExchangeQuestClient
```

## Testing
The project includes unit tests for order matching and other functionalities. To run the tests, follow these steps:

```sh
make tests
```

## LICENSE
This project is licensed under the [MIT License](LICENSE). Feel free to modify and distribute it as per the terms of the license.