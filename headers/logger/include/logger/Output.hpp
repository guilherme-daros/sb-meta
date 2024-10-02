#pragma once

#include <iostream>
#include <ostream>

namespace output {

class Base {
  virtual auto stream() -> std::ostream& = 0;
};

class Console : public Base {
 public:
  auto stream() -> std::ostream& override {
    static auto& pStream = std::cout;
    return pStream;
  }
};
}  // namespace output
