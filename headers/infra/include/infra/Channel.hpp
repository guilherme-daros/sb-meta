#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <optional>

#include "CircularBuffer.hpp"

namespace infra {

template <typename T, size_t Size = 1>
class Channel {
 public:
  auto send(T value) -> void {
    std::unique_lock lock(mtx_);

    cv_full_.wait(lock, [this] { return !buffer_.full() || closed_; });

    if (closed_) {
      throw std::runtime_error("Cannot send on closed channel");
    }

    buffer_.push(std::move(value));
    cv_empty_.notify_one();
  }

  friend auto operator<<(Channel& ch, T value) -> Channel& {
    ch.send(value);
    return ch;
  }

  friend auto operator>>(Channel& ch, std::optional<T>& value) -> Channel& {
    value = ch.receive();
    return ch;
  }

  template <typename Rep, typename Period>
  auto receive(const std::chrono::duration<Rep, Period>& timeout) -> std::optional<T> {
    std::unique_lock lock(mtx_);

    bool received = cv_empty_.wait_for(lock, timeout, [this] { return !buffer_.empty() || closed_; });

    if (!received || (buffer_.empty() && closed_)) return std::nullopt;

    auto value = buffer_.pop();
    cv_full_.notify_one();
    return value;
  }

  auto receive() -> std::optional<T> {
    std::unique_lock lock(mtx_);

    cv_empty_.wait(lock, [this] { return !buffer_.empty() || closed_; });

    if (buffer_.empty() && closed_) return std::nullopt;

    auto value = buffer_.pop();
    cv_full_.notify_one();
    return value;
  }

  auto close() -> void {
    std::lock_guard lock(mtx_);
    closed_ = true;
    cv_empty_.notify_all();
    cv_full_.notify_all();
  }

 private:
  bool closed_ = false;
  std::mutex mtx_;
  std::condition_variable cv_empty_;
  std::condition_variable cv_full_;
  CircularBuffer<T, Size> buffer_;
};
}  // namespace infra
