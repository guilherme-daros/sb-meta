
#include <atomic>
#include <chrono>
#include <functional>
#include <future>
#include <thread>

#include <unistd.h>

namespace infra {
class Monitor {
  using Dr = std::chrono::duration<double>;
  using Fn = std::function<void()>;

 public:
  Monitor(Dr interval) : interval_(interval) {}
  Monitor(Dr interval, Fn func) : interval_(interval), func_(func) {}

  virtual ~Monitor() {
    future_.wait();
    // Stop();
  };

  auto SetRunnable(Fn func) { func_ = func; }

  auto Start() -> void {
    if (monitoring_) return;

    monitoring_ = true;
    future_ = std::async(std::launch::async, [this] { DoMonitor(); });
  }

  auto Stop() -> void { monitoring_ = false; }

  auto DoMonitor() -> void {
    while (monitoring_) {
      auto next_run = std::chrono::steady_clock::now() + interval_;
      func_();
      std::this_thread::sleep_until(next_run);
    }
  }

 private:
  std::chrono::duration<double> interval_;
  std::atomic_bool monitoring_ = false;
  std::function<void()> func_;
  std::future<void> future_;
};
}  // namespace infra
