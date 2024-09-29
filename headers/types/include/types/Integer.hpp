#pragma once

#include <cstdint>
#include <limits>
#include <ostream>

namespace types {
static constexpr uint64_t bitwidth_calc_(uint64_t d) { return d < 2 ? d : 1 + bitwidth_calc_(d >> 1); }

template <typename T, T M__ = std::numeric_limits<T>::max(), T m__ = std::numeric_limits<T>::min()>
class Integer {
  static_assert(m__ <= M__, "Integer: `max` value should be higher than `min`");

 public:
  Integer(T value = 0) : value_(value) {}

  using type = T;

  operator T() { return value_; }

  Integer &operator=(const T &v) {
    value_ = v;
    return *this;
  }

  friend std::ostream &operator<<(const Integer &i, std::ostream &os) {
    os << i.value_;
    return os;
  }

  friend bool operator==(const Integer &i1, const Integer &i2) { return i1.value_ == i2.value_; }

  friend bool operator!=(const Integer &i1, const Integer &i2) { return !(i1.value_ == i2.value_); }

  static constexpr T max() { return max_; }
  static constexpr T min() { return min_; }

  auto value() -> T { return value_; }

  auto set_value(T v) -> void { value_ = v; }

  static constexpr uint16_t bitwidth() { return bitwidth_; }
  static constexpr T range() { return M__ - m__ + 1; }

 protected:
  T value_;
  static constexpr uint8_t bitwidth_ = bitwidth_calc_((uint64_t)(M__)-m__);
  static constexpr T max_ = T{M__};
  static constexpr T min_ = T{m__};
};
}  // namespace types
