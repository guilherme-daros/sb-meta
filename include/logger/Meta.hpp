#pragma once

#include <type_traits>

namespace sb::logger::meta {

template <typename Base, template <typename> typename Concept, typename... Types>
struct TypeFinder;

template <typename Base, template <typename> typename Concept, typename First, typename... Rest>
struct TypeFinder<Base, Concept, First, Rest...> {
  using type = std::conditional_t<Concept<First>::value, First, typename TypeFinder<Base, Concept, Rest...>::type>;
};

template <typename Base, template <typename> typename Concept>
struct TypeFinder<Base, Concept> {
  using type = Base::Default;
};

template <typename Base, template <typename> typename Concept, typename... Types>
using TypeFinder_t = typename TypeFinder<Base, Concept, Types...>::type;
}  // namespace sb::logger::meta
