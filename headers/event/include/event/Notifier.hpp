#pragma once

#include <typeinfo>

#include "Listener.hpp"
#include "types/Singleton.hpp"

namespace event {

class Notifier : public types::Singleton<Notifier> {
 public:
  template <typename L, typename T>
  auto AddListener(T* listener) -> Notifier& {
    auto& list = Listener<T>::notify_list();
    list.insert({typeid(L).hash_code(), listener});

    return *this;
  }

  template <typename L, typename T>
  auto RemoveListener(T* listener) -> Notifier& {
    auto& list = Listener<T>::notify_list();
    list.erase(typeid(L).hash_code());

    return *this;
  }

  template <typename T, typename... Args>
    requires std::invocable<T, Args...>
  auto Notify(Args... args) -> Notifier& {
    Listener<T>::Notify(args...);

    return *this;
  }
};
}  // namespace event
