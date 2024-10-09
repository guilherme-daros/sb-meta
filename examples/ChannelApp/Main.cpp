#include <future>
#include <iostream>
#include <thread>
#include "infra/Channel.hpp"

using namespace std::chrono_literals;

int main() {
  auto running = std::atomic_bool(true);
  auto ch = infra::Channel<int>();

  auto runnable = [&running, &ch] {
    std::cout << "ThreadId: " << std::this_thread::get_id() << std::endl;

    while (running) {
      std::cout << "Worker waiting for message" << std::endl;
      auto value = ch.receive();

      if (value.has_value()) {
        std::this_thread::sleep_for(1s);
        std::cout << "Worker received message" << std::endl;
      } else {
        break;
      }
    }
  };

  auto th = std::async(std::launch::async, runnable);

  for (int i = 1; i <= 3; ++i) ch.send(i);

  std::this_thread::sleep_for(1s);
  running = false;

  return 0;
}
