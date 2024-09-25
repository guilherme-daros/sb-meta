#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <ostream>
#include "infra/Logger.hpp"

namespace {
using Main = logger::Logger<logger::Adaptor, "main">;
using Test = logger::Logger<logger::Adaptor, "test">;
}  // namespace

int main(int argc, char *argv[]) {
  Main::sDebug() << "Help" << std::setw(10) << std::right << "Romero" << std::endl;
  Main::sInfo() << "Help" << std::setw(10) << std::right << "Romero" << std::endl;
  Main::sWarning() << "Help" << std::setw(10) << std::right << "Romero" << std::endl;
  Main::sError() << "Help" << std::setw(10) << std::right << "Romero" << std::endl;

  Test::sDebug() << "Help" << std::setw(10) << std::right << "Romero" << std::endl;
  Test::sInfo() << "Help" << std::setw(10) << std::right << "Romero" << std::endl;
  Test::sWarning() << "Help" << std::setw(10) << std::right << "Romero" << std::endl;
  Test::sError() << "Help" << std::setw(10) << std::right << "Romero" << std::endl;

  return 0;
}
