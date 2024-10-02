#pragma once

#include <memory>
#include "Connection.hpp"

namespace event {

template <typename... Args>
class Signal {
 public:
  using Slot = std::function<void(Args...)>;

  template <typename T>
  Connection connect(std::weak_ptr<T> weak_observer, void (T::*method)(Args...)) {
    std::lock_guard lock(mutex);

    Slot slot = [weak_observer, method](Args... args) {
      if (auto shared_observer = weak_observer.lock()) {
        (shared_observer.get()->*method)(args...);
      }
    };

    slots.push_back(slot);
    auto slot_it = std::prev(slots.end());

    return Connection([this, slot_it]() {
      std::lock_guard lock(mutex);
      slots.erase(slot_it);
    });
  }

  void emit(Args... args) {
    std::lock_guard lock(mutex);
    for (auto& slot : slots) {
      slot(args...);
    }
  }

 private:
  std::vector<Slot> slots;
  std::mutex mutex;
};

}  // namespace event
