#include <iomanip>
#include <iostream>
#include "notifier/Listener.hpp"

class RebootListener {
 public:
  virtual ~RebootListener() = default;
  virtual auto OnRebootNotification(bool done) -> void = 0;
  void operator()(bool done) { OnRebootNotification(done); }
};

class PowerUpListener {
 public:
  virtual ~PowerUpListener() = default;
  virtual auto OnPowerUpNotification(bool done) -> void = 0;
  void operator()(bool done) { OnPowerUpNotification(done); }
};
class ConnectListener {
 public:
  virtual ~ConnectListener() = default;
  virtual auto OnConnect(bool done) -> void = 0;
  void operator()(bool done) { OnConnect(done); }
};
class BootListener {
 public:
  virtual ~BootListener() = default;
  virtual auto OnBootNotification(bool done) -> void = 0;
  void operator()(bool done) { OnBootNotification(done); }
};

using Not = notifier::Notifier<notifier::Listener<RebootListener>, notifier::Listener<BootListener>,
                               notifier::Listener<PowerUpListener>, notifier::Listener<ConnectListener>>;

class Manager : public RebootListener, BootListener, PowerUpListener {
 public:
  Manager() {
    auto& n = Not::Instance();
    n.AddListener<Manager, RebootListener>(this)
        .AddListener<Manager, BootListener>(this)
        .AddListener<Manager, PowerUpListener>(this);
  }

  ~Manager() {
    auto& n = Not::Instance();
    n.RemoveListener<Manager, RebootListener>(this)
        .RemoveListener<Manager, BootListener>(this)
        .RemoveListener<Manager, PowerUpListener>(this);
  }

 private:
  auto OnRebootNotification(bool done) -> void override {
    std::cout << std::setw(20) << std::left << "Manager" << ": Reboot Notification" << std::endl;
  }

  auto OnBootNotification(bool done) -> void override {
    std::cout << std::setw(20) << std::left << "Manager" << ": Boot Notification" << std::endl;
  }
  auto OnPowerUpNotification(bool done) -> void override {
    std::cout << std::setw(20) << std::left << "Manager" << ": PowerUp Notification" << std::endl;
  }
};

class Coordinator : public RebootListener {
 public:
  Coordinator() {
    auto& n = Not::Instance();
    n.AddListener<decltype(this), RebootListener>(this);
  }

  ~Coordinator() {
    auto& n = Not::Instance();
    n.RemoveListener<decltype(this), RebootListener>(this);
  }

 private:
  auto OnRebootNotification(bool done) -> void override {
    std::cout << std::setw(20) << std::left << "Coordinator" << ": Reboot Notification" << std::endl;
  }
};

auto main(int argc, char* argv[]) -> int {
  auto& notifier = Not::Instance();

  notifier.NotifyT<ConnectListener>(false);

  auto c = Coordinator();
  notifier.NotifyT<RebootListener>(false);
  {
    auto m = Manager();
    // Received by Manager and Coordinator
    notifier.NotifyT<RebootListener>(true);

    // Received by Manager only
    notifier.NotifyT<BootListener>(true);
  }
  // Received by Coordinator
  notifier.NotifyT<RebootListener>(true);

  // Received by none
  notifier.NotifyT<BootListener>(true);

  return 0;
}
