#include <thread>

#include "logger/Logger.hpp"

namespace {
using Main = sb::logger::Logger<"Main">;
}

auto runnable(const int id) {
  for (auto i = 0; i < 1e6; i++) {
    Main::Info() << id << " Logging: " << i << std::endl;
  }
};

int main(int argc, char* argv[]) {
  std::array<std::thread, 100> th;

  for (auto i = 0; i < 100; i++) {
    th[i] = std::thread(runnable, i);
  }

  for (auto i = 0; i < 100; i++) {
    th[i].join();
  }

  return 0;
}
