#pragma once

#include <type_traits>

namespace sb::meta {

template <typename Base, typename... Types>
struct TypeFinder;

template <typename Base, typename First, typename... Rest>
struct TypeFinder<Base, First, Rest...> {
  using type =
      std::conditional_t<Base::template Condition<First>::value, First, typename TypeFinder<Base, Rest...>::type>;
};

template <typename Base>
struct TypeFinder<Base> {
  using type = Base::Default;
};

template <typename Base, typename... Types>
using TypeFinder_t = typename TypeFinder<Base, Types...>::type;
}  // namespace sb::meta
