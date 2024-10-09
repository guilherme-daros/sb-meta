#include <unistd.h>
#include <chrono>
#include <iostream>
#include "infra/Monitor.hpp"

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  auto m = infra::Monitor(2s, [] { std::cout << "Help me" << std::endl; });

  m.Start();

  return 0;
}
