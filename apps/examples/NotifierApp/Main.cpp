#include <iostream>
#include <memory>

#include "notifier/Connection.hpp"
#include "notifier/Signal.hpp"
#include "types/Singleton.hpp"

template <typename... Ts>
struct Event {
  using Signal = event::Signal<Ts...>;
};

using RechargeEvent = Event<int, int, int>;

class Notifier : public types::Singleton<Notifier> {
 public:
  template <typename... Args>
  auto NotifyRechargeEvent(Args... args) -> void {
    recharge_event_.emit(args...);
  }
  auto recharge_event() -> RechargeEvent::Signal& { return recharge_event_; }

 private:
  RechargeEvent::Signal recharge_event_;
};

class Listener : public std::enable_shared_from_this<Listener> {
 public:
  auto Init() -> std::shared_ptr<Listener> {
    auto& signal = Notifier::Instance().recharge_event();
    connection_ = signal.connect<Listener>(shared_from_this(), &Listener::onEvent);
    return shared_from_this();
  }

  void onEvent(int x, int y, int z) { std::cout << "Observer received value: " << x << std::endl; }

 private:
  event::Connection connection_;
};

int main() {
  auto& n = Notifier::Instance();

  auto observer = std::make_shared<Listener>()->Init();

  std::cout << observer.use_count() << std::endl;

  auto ob = observer;

  std::cout << observer.use_count() << std::endl;
  n.NotifyRechargeEvent(1, 2, 4);

  return 0;
};
