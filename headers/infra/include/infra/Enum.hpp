#include "infra/Integer.hpp"

template <typename T, T M__, T m__>
class Enum : public Integer<uint32_t, static_cast<uint32_t>(M__), static_cast<uint32_t>(m__)> {
 public:
  Enum(T value = m__) : Integer<int, static_cast<int>(M__), static_cast<int>(m__)>(static_cast<int>(value)) {}
  Enum(int value) : Integer<int, static_cast<int>(M__), static_cast<int>(m__)>(value) {}

  operator T() { return static_cast<T>(this->value()); }
  using values = T;

  friend bool operator==(const Enum &i1, const T &i2) { return i1.value_ == static_cast<int>(i2); }

  friend bool operator!=(const Enum &i1, const T &i2) { return !(i1.value_ == static_cast<int>(i2)); }

  static constexpr T max() { return M__; }
  static constexpr T min() { return m__; }
};
