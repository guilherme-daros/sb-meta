#include "logger/Logger.hpp"
#include "logger/Timing.hpp"

namespace {
using timestamp_logger = sb::logger::Logger<"Main", sb::logger::timing::Timestamp>;
using uptime_logger = sb::logger::Logger<"Main", sb::logger::timing::Uptime>;
}  // namespace

int main(int argc, char* argv[]) {
  timestamp_logger::Debug() << "This has a timestamp" << std::endl;
  uptime_logger::Debug() << "This has an uptime " << std::endl;
  return 0;
}
