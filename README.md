# Ethereum PendingTransaction Watcher

## Description

Ethereum Pending Transaction Watcher is a C++ application that uses the Infura API to monitor and fetch details of new pending transactions on the Ethereum blockchain. The application uses WebSocket for real-time updates and HTTP for fetching transaction details.

## Features

- Connects to Ethereum Mainnet via Infura's WebSocket API
- Subscribes to new pending transactions
- Fetches detailed information of a new pending transaction

## Requirements

- C++11 or higher
- CMake 3.10 or higher
- OpenSSL 3
- C++ REST SDK (cpprest)
- Boost libraries

## Setup and Installation

### macOS (Homebrew)

Install the necessary libraries:

```bash
brew install openssl@3 cpprestsdk boost
```

### Ubuntu (apt)

```bash
sudo apt-get install libcpprest-dev libssl-dev libboost-all-dev
```

## Compilation

A Makefile is provided for easier compilation. Navigate to the project directory and run:

```bash
make all
```

### Makefile Explained

- `CXX` sets the C++ compiler, which is `g++` by default.
- `CXXFLAGS` sets the C++ version to C++11.
- `INCLUDES` sets the include paths for the cpprestsdk, boost and openssl.
- `LIBS` sets the library paths for the cpprestsdk, boost and openssl.
- `LINK_FLAGS` sets the libraries to link during the compile process.

## How to Run

After successful compilation, run the application:

```bash
./main
```

## Usage

When running the application, it will establish a WebSocket connection with Infura's Ethereum Mainnet endpoint. Once connected, it will start subscribing to new pending transactions. The detailed information of each new pending transaction will be fetched and displayed.

## How it Works

- `main()` initializes the WebSocket client and connects to Infura's Ethereum endpoint.
- It then sends a subscription message to start receiving updates for new pending transactions.
- For each new pending transaction, `fetch_transaction_details()` is invoked on a new thread to fetch the transaction details via HTTP.

## Contributing

Feel free to contribute to this project by submitting pull requests or issues.

## License

This project is under the MIT License. See the LICENSE.md file for details.
