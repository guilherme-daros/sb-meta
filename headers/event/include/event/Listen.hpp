#pragma once

#include "Notifier.hpp"

namespace event {
template <typename T, typename... Ts>
class Listen : public Ts... {
 protected:
  explicit Listen(T* l) : Ts()... {
    auto& notifier = event::Notifier::Instance();

    (notifier.AddListener<T, Ts>(l), ...);
  }

  ~Listen() {
    auto& notifier = event::Notifier::Instance();

    (notifier.RemoveListener<T, Ts>(static_cast<T*>(this)), ...);
  }
};
}  // namespace event
