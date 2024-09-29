
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

  ~Monitor() {
    Stop();
    future_.wait();
  };

  auto SetRunnable(Fn func) { func_ = func; }

  auto Start() -> void {
    if (monitoring_) return;

    monitoring_ = true;
    future_ = std::async(std::launch::async, [this] { Run(); });
  }

  auto Stop() -> void { monitoring_ = false; }

  auto Run() -> bool {
    while (monitoring_) {
      auto next_run = std::chrono::steady_clock::now() + interval_;
      func_();
      std::this_thread::sleep_until(next_run);
    }
    return true;
  }

 private:
  std::atomic_bool monitoring_ = false;
  std::future<void> future_;
  Dr interval_;
  Fn func_;
};
}  // namespace infra
