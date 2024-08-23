#include <array>
#include <cstddef>
#include <optional>

namespace infra {

template <typename T, size_t Size>
class CircularBuffer {
 public:
  CircularBuffer() : head_(0), tail_(0), count_(0) {}

  auto push(T value) -> bool {
    if (count_ == Size) return false;

    buffer_[tail_] = std::move(value);
    tail_ = (tail_ + 1) % Size;
    ++count_;

    return true;
  }

  auto pop() -> std::optional<T> {
    if (count_ == 0) return std::nullopt;

    T value = std::move(buffer_[head_]);
    head_ = (head_ + 1) % Size;
    --count_;

    return value;
  }

  auto empty() const -> size_t { return count_ == 0; }

  auto full() const -> size_t { return count_ == Size; }

 private:
  std::array<T, Size> buffer_;
  size_t head_;
  size_t tail_;
  size_t count_;
};
}  // namespace infra
