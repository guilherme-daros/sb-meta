#pragma once

#include <iostream>
#include <ostream>

#include "logger/Meta.hpp"

namespace logger::output {

class Console;

class Base {
 public:
  using Default = Console;
  virtual auto stream() -> std::ostream& = 0;
};

template <typename T>
struct is_output : std::is_base_of<output::Base, T> {};

template <typename... Ts>
using Output = meta::TypeFinder_t<output::Base, output::is_output, Ts...>;

class Console : public Base {
 public:
  auto stream() -> std::ostream& override {
    static auto& pStream = std::cout;
    return pStream;
  }
};

}  // namespace logger::output
