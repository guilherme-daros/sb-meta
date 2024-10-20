#include <iostream>
#include <string_view>
#include <type_traits>

#include "meta/TypeFinder.hpp"

using namespace std::string_view_literals;

namespace message {

class English;

class Base {
 public:
  template <typename T>
  using Condition = std::is_base_of<Base, T>;
  using Default = English;

  virtual auto Message() const -> std::string_view = 0;
};

template <typename... Ts>
using Message = sb::meta::TypeFinder_t<Base, Ts...>;

};  // namespace message

class English : public message::Base {
 public:
  auto Message() const -> std::string_view override { return "Help"sv; }
};

class Portuguese : public message::Base {
 public:
  auto Message() const -> std::string_view override { return "Ajuda"sv; }
};

int main(int argc, char *argv[]) {
  using Msg = message::Message<int, float, Portuguese, char>;

  auto msg = Msg();

  std::cout << msg.Message() << std::endl;
  return 0;
}
