#pragma once

namespace types {
template <class T>
class Singleton {
 public:
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;

  auto static Instance() -> T & {
    if (!instance_) instance_ = new _T;
    return *instance_;
  }

 protected:
  Singleton() {}

 private:
  struct _T : public T {
    _T() : T() {}
  };

  static inline T *instance_ = nullptr;
};
}  // namespace types
