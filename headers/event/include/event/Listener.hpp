#pragma once

#include <unordered_map>

namespace event {

class Notifier;

template <typename T>
class Listener {
  using List = std::unordered_map<std::size_t, T*>;

 protected:
  static auto Notify(auto... args) -> void {
    for (auto listener : notify_list_) {
      (*std::get<1>(listener))(args...);
    }
  }
  static auto notify_list() -> List& { return notify_list_; }
  friend class event::Notifier;

 private:
  static List notify_list_;
};

template <typename T>
Listener<T>::List Listener<T>::notify_list_;

}  // namespace event
