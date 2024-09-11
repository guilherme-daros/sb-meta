#include "infra/Log.hpp"

int main(int argc, char *argv[]) {
  Log::reporting_level(Log::Level::INFO);

  Stream_ERROR("Main") << "Help" << std::endl;
  Stream_WARNING("Main") << "Help" << std::endl;
  Stream_INFO("Main") << "Help" << std::endl;
  Stream_DEBUG("Main") << "Help" << std::endl;
  return 0;
}
