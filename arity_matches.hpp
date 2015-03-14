#ifndef _ARITY_MATCHES_HPP_
#define _ARITY_MATCHES_HPP_

#include "tuple_arity.hpp"
#include "arity_increasing.hpp"

template<typename T, typename ...A>
struct arity_matches {
   static const bool value = !arity_increasing<A...>::value && (tuple_arity<T>::value == sizeof...(A));
};

#endif//_ARITY_MATCHES_HPP_
