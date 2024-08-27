#include <iostream>
#include <thread>
#include "infra/Channel.hpp"

void worker(int id, infra::Channel<int> &ch) {
  std::cout << "Worker starting" << std::endl;

  using namespace std::chrono_literals;

  while (true) {
    std::cout << "Worker waiting for message" << std::endl;
    auto value = ch.receive(1s);

    if (value.has_value()) {
      std::cout << "Worker received message" << std::endl;
    } else {
      std::cout << "Worker timeout" << std::endl;
      break;
    }
  }
}

int main() {
  auto ch = infra::Channel<int>();
  auto t1 = std::thread(worker, 1, std::ref(ch));

  for (int i = 1; i <= 3; ++i) ch.send(i);

  t1.join();

  ch.close();

  return 0;
}
