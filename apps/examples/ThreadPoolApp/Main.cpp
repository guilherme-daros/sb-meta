#include <iostream>
#include <mutex>
#include "infra/ThreadPool.hpp"

int main(int argc, char* argv[]) {
  auto th = infra::ThreadPool<5>();
  auto mtx = std::mutex();

  auto task = [&mtx](int i) {
    std::scoped_lock lock{mtx};
    std::cout << i << std::endl;
  };

  for (int n = 0; n < 1000; ++n) {
    th.AddTask(task, n);
  }

  while (true) pause();
}
