#include <cstdint>
#include <typeinfo>
#include <unordered_map>

#include "types/Singleton.hpp"

namespace notifier {

template <typename T>
class Listener {
  using List = std::unordered_map<uint64_t, T*>;

 protected:
  static auto Notify(auto... args) -> void {
    for (auto listener : notify_list_) {
      (*std::get<1>(listener))(args...);
    }
  }
  static auto notify_list() -> List& { return notify_list_; }

 private:
  static List notify_list_;
};

template <typename T>
Listener<T>::List Listener<T>::notify_list_;

template <typename... Ls>
class Notifier : public types::Singleton<Notifier<Ls...>>, Ls... {
 public:
  template <typename L, typename T>
  auto AddListener(T* listener) -> Notifier& {
    // std::cout << "Adding: " << typeid(L).name() << " as " << typeid(T).name() << std::endl;
    Listener<T>::notify_list().insert({typeid(L).hash_code(), listener});
    return *this;
  }

  template <typename L, typename T>
  auto RemoveListener(T* listener) -> Notifier& {
    // std::cout << "Removing: " << typeid(L).name() << " as " << typeid(T).name() << std::endl;
    Listener<T>::notify_list().erase(typeid(L).hash_code());
    return *this;
  }

  template <typename T>
  auto NotifyT(auto... args) -> Notifier& {
    Listener<T>::Notify(args...);
    return *this;
  }
};
}  // namespace notifier
