# ExchangeQuest

- [ExchangeQuest](#exchangequest)
  - [Introduction](#introduction)
  - [Requirements](#requirements)
  - [Building and Running](#building-and-running)
  - [Testing](#testing)
  - [Usage](#usage)
    - [1. Starting the Client and Server](#1-starting-the-client-and-server)
    - [2. Connecting to the Server](#2-connecting-to-the-server)
    - [3. Registering a User](#3-registering-a-user)
    - [4. Logging In](#4-logging-in)
    - [5. Creating a Order](#5-creating-a-order)
    - [6. Checking Balance](#6-checking-balance)
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
./build/server/ExchangeQuestServer
```

To run the client:
```sh
./build/client/ExchangeQuestClient
```

## Testing
The project includes unit tests for order matching and other functionalities. To run the tests, follow these steps:

```sh
make tests
```

## Usage

### 1. Starting the Client and Server

Start the server:
```sh
./build/server/ExchangeQuestServer
```

Start the client:
```sh
./build/client/ExchangeQuestClient
```

### 2. Connecting to the Server

When you start the client, you'll be presented with a menu. Choose the option to connect to the server by selecting `1`.

```
 --------------------------------------------------------------
|                         Exchange 1.0                         |
 --------------------------------------------------------------
 --------------------------------------------------------------
|                       Select menu item                       |
 --------------------------------------------------------------
| 0. Exit                                                      |
| 1. Connect to the server                                     |
| 2. Registration                                              |
| 3. Login                                                     |
| 4. Order                                                     |
| 5. Balance                                                   |
 --------------------------------------------------------------
```

### 3. Registering a User

To register, select option `2`.

```
 --------------------------------------------------------------
|                       Select menu item                       |
 --------------------------------------------------------------
| 0. Exit                                                      |
| 1. Connect to the server                                     |
| 2. Registration                                              |
| 3. Login                                                     |
| 4. Order                                                     |
| 5. Balance                                                   |
 --------------------------------------------------------------
 
```

### 4. Logging In

After registration, you can log in by selecting option `3` and entering your UID.

```
 --------------------------------------------------------------
|                       Select menu item                       |
 --------------------------------------------------------------
| 0. Exit                                                      |
| 1. Connect to the server                                     |
| 2. Registration                                              |
| 3. Login                                                     |
| 4. Order                                                     |
| 5. Balance                                                   |
 --------------------------------------------------------------

Enter UID:
```

### 5. Creating a Order

To create a new trading order, select option `4` and follow the format for entering your order details.

```
 --------------------------------------------------------------
|                       Select menu item                       |
 --------------------------------------------------------------
| 0. Exit                                                      |
| 1. Create a new trading order                                |
|    Format: [INSTRUMENT]|[SIDE]:[VOLUME]@[PRICE]              |
|    INSTRUMENT: Trading instrument (e.g., USD)                |
|    SIDE: BUY (for buying) or SELL (for selling)              |
|    VOLUME: Number of units to trade                          |
|    PRICE: Price per unit                                     |
|                                                              |
|    Example: USD|BUY:100@150.25                               |
 --------------------------------------------------------------

Enter your order:
```

### 6. Checking Balance

To check your balance, select option `5`.

```
 --------------------------------------------------------------
|                       Select menu item                       |
 --------------------------------------------------------------
| 0. Exit                                                      |
| 1. Connect to the server                                     |
| 2. Registration                                              |
| 3. Login                                                     |
| 4. Order                                                     |
| 5. Balance                                                   |
 --------------------------------------------------------------

Checking balance...
```


## LICENSE
This project is licensed under the [MIT License](LICENSE). Feel free to modify and distribute it as per the terms of the license.
