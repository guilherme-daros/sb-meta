#include <iostream>

#include "notifier/Connection.hpp"
#include "notifier/Signal.hpp"

class Observer : public std::enable_shared_from_this<Observer> {
 public:
  Observer(notifier::Signal<int>& signal) { connection = signal.connect(shared_from_this(), &Observer::onEvent); }

  void onEvent(int value) { std::cout << "Observer received value: " << value << std::endl; }

 private:
  Connection connection;
};

int main() {
  notifier::Signal<int> signal;

  auto observer = std::make_shared<Observer>(signal);

  signal.emit(42);

  observer.reset();

  signal.emit(42);

  return 0;
}
