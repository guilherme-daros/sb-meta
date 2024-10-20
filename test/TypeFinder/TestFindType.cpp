#include <gtest/gtest.h>
#include <string_view>

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

class English : public Base {
 public:
  auto Message() const -> std::string_view override { return "Help"sv; }
};

class Portuguese : public Base {
 public:
  auto Message() const -> std::string_view override { return "Ajuda"sv; }
};

template <typename... Ts>
using Message = sb::meta::TypeFinder_t<Base, Ts...>;

}  // namespace message

TEST(TypeFinder, FindType) {
  using Msg = message::Message<int, message::Portuguese, char>;
  auto msg = Msg();

  ASSERT_EQ(msg.Message(), "Ajuda"sv);
}
