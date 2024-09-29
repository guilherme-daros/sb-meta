#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <ostream>
#include "infra/Logger.hpp"

namespace {
using Main = logger::Logger<logger::output::Console, "main">;
}  // namespace

int main(int argc, char *argv[]) {
  Main::Debug() << "Help" << std::endl;
  Main::Info() << "Help" << std::endl;
  Main::Warning() << "Help" << std::endl;
  Main::Error() << "Help" << std::endl;

  return 0;
}
