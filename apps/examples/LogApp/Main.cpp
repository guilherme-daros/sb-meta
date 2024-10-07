#include <unistd.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <thread>

#include "logger/Logger.hpp"
#include "logger/Output.hpp"
#include "logger/Timing.hpp"

namespace {
using Main = logger::Logger<"main", logger::timing::Uptime, logger::output::Console>;
}  // namespace

auto rand_delay() -> void { std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000)); }

void help() {
  while (true) {
    rand_delay();
    Main::Debug() << "Help" << std::endl;
    rand_delay();
    Main::Info() << "Help" << std::endl;
    rand_delay();
    Main::Warning() << "Help" << std::endl;
    rand_delay();
    Main::Error() << "Help" << std::endl;
  }
}
int main(int argc, char *argv[]) {
  srand((unsigned)time(NULL));

  auto t1 = std::thread(help);
  auto t2 = std::thread(help);

  t1.join();
  t2.join();
  return 0;
}
