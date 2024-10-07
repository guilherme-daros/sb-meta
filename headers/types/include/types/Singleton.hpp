#pragma once

namespace types {
template <class T>
class Singleton {
 public:
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  Singleton(Singleton &&) = delete;
  Singleton &operator=(Singleton &&) = delete;

  auto static Instance() -> T & {
    static T instance_;
    return instance_;
  }

 protected:
  Singleton() {}

  static inline T *instance_ = nullptr;
};
}  // namespace types
