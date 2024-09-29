#pragma once

#include <functional>

class Connection {
 public:
  Connection(std::function<void()> disconnect_func) : disconnect_func(disconnect_func) {}

  void disconnect() {
    if (disconnect_func) {
      disconnect_func();
      disconnect_func = nullptr;
    }
  }

 private:
  std::function<void()> disconnect_func;
};
