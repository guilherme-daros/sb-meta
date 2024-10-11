#include <iomanip>
#include <iostream>
#include "event/Listen.hpp"
#include "event/Notifier.hpp"

class RebootEvent {
 public:
  virtual ~RebootEvent() = default;
  virtual auto OnReboot(bool done) -> void = 0;
  auto operator()(bool done) -> void { OnReboot(done); }
};

class BootEvent {
 public:
  virtual ~BootEvent() = default;
  virtual auto OnBoot(bool done) -> void = 0;
  auto operator()(bool done) -> void { OnBoot(done); }
};

class Manager final : public event::Listen<Manager, RebootEvent, BootEvent> {
 public:
  Manager() : Listen(this) {}

 private:
  auto OnReboot(bool done) -> void override {
    std::cout << std::setw(20) << std::left << "Manager" << ": Reboot Notification" << std::endl;
  }
  auto OnBoot(bool done) -> void override {
    std::cout << std::setw(20) << std::left << "Manager" << ": Boot Notification" << std::endl;
  }
};

auto main(int argc, char* argv[]) -> int {
  auto& notifier = event::Notifier::Instance();

  {
    auto m = Manager();
    notifier.Notify<RebootEvent>(false);
  }

  notifier.Notify<BootEvent>(false);

  return 0;
}
