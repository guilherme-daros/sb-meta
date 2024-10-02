#pragma once

#include <functional>
namespace event {

class Connection {
 public:
  Connection() : disconnect_func(nullptr) {}
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
}  // namespace event
