#include <unistd.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <thread>
#include "logger/Logger.hpp"

namespace {
using Main = logger::Logger<"main">;
}

int main(int argc, char *argv[]) {
  Main::logging_level = logger::Level::Debug;
  srand((unsigned)time(NULL));

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
    Main::Debug() << "Help" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
    Main::Info() << "Help" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
    Main::Warning() << "Help" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
    Main::Error() << "Help" << std::endl;
  }
  return 0;
}
