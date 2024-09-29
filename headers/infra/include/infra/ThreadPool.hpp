#include <array>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <utility>

#include "CircularBuffer.hpp"

namespace infra {
template <size_t Size>
class ThreadPool {
 public:
  ThreadPool() : busy_threads(Size), threads_(std::array<std::thread, Size>()) {
    for (auto& th : threads_) {
      th = std::thread(ThreadWorker(this));
    }
  }

  ~ThreadPool() { Shutdown(); }

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;

  auto operator=(const ThreadPool&) = delete;
  auto operator=(ThreadPool&&) = delete;

  auto Shutdown() -> void {
    {
      std::scoped_lock lock{mtx_};
      shutdown_requested_ = true;
      cv_.notify_all();
    }
    for (auto& th : threads_) {
      if (th.joinable()) th.join();
    }
  }

  template <typename Fn, typename... Args>
  auto AddTask(Fn&& fn, Args&&... args) -> std::future<decltype(fn(args...))> {
    auto func = std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...);

    auto task_ptr = std::make_shared<std::packaged_task<decltype(fn(args...))()>>(func);

    auto wrapper = [task_ptr] { (*task_ptr)(); };

    {
      std::scoped_lock lock{mtx_};
      // This push may fail, need to figure how to keep trying
      task_queue_.push(wrapper);
      cv_.notify_one();
    };

    return task_ptr->get_future();
  }

  auto QueueSize() -> size_t {
    std::scoped_lock lock{mtx_};
    return task_queue_.size();
  }

  class ThreadWorker {
   public:
    ThreadWorker(ThreadPool* thread_pool) : thread_pool_(thread_pool) {}

    auto operator()() -> void {
      std::unique_lock lock{thread_pool_->mtx_};
      while (!thread_pool_->shutdown_requested_ ||
             (thread_pool_->shutdown_requested_ && !thread_pool_->task_queue_.empty())) {
        thread_pool_->busy_threads--;
        thread_pool_->cv_.wait(
            lock, [this] { return thread_pool_->shutdown_requested_ || !thread_pool_->task_queue_.empty(); });
        thread_pool_->busy_threads++;

        if (!thread_pool_->task_queue_.empty()) {
          auto func = thread_pool_->task_queue_.pop();
          if (func.has_value()) {
            lock.unlock();
            func.value()();
            lock.lock();
          }
        }
      }
    }

   private:
    ThreadPool* thread_pool_;
  };

 public:
  uint32_t busy_threads;

 private:
  bool shutdown_requested_ = false;

  mutable std::mutex mtx_;
  std::condition_variable cv_;

  std::array<std::thread, Size> threads_;
  infra::CircularBuffer<std::function<void()>, Size * 5> task_queue_;
};
}  // namespace infra
