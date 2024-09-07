#include <chrono>
#include <iostream>
#include "infra/Monitor.hpp"
#include "infra/Singleton.hpp"

using namespace std::chrono_literals;

class Manager : public infra::Singleton<Manager> {
 public:
  auto Init() -> void { monitor_.Start(); }

 protected:
  Manager() : monitor_(300ms) {
    monitor_.SetRunnable([] { std::cout << "Monitor from manager" << std::endl; });
  }

 private:
  infra::Monitor monitor_;
};

int main(int argc, char* argv[]) {
  auto& manager = Manager::Instance();

  manager.Init();

  auto start_time = std::chrono::high_resolution_clock::now();

  auto m = infra::Monitor(2s);
  m.SetRunnable([&start_time] {
    auto current_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);

    std::cout << "Program has been running for " << duration.count() << " seconds" << std::endl;
  });

  m.Start();

  return 0;
}
