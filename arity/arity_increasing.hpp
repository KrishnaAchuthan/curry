#ifndef _ARITY_INCREASING_HPP_
#define _ARITY_INCREASING_HPP_

#include "..\placeholders\placeholder.hpp"
#include "..\util\fn_fwd.hpp"

template<typename ...A>
struct arity_increasing;

template<typename A, typename ...Rest>
struct arity_increasing<A, Rest...> {
   //static const bool value = (function_traits<A>::arity > 0) || arity_increasing<Rest...>::value;
   static const bool value = arity_increasing<Rest...>::value;
};

template<>
struct arity_increasing<> {
   static const bool value = false;
};

template<typename F, typename ...Rest>
struct arity_increasing<cfn_t<F>, Rest...> {
   static const bool value = true;
};

template<int I, typename ...Rest>
struct arity_increasing<ph<I>, Rest...> {
   static const bool value = true;
};

template<int I, typename ...Rest>
struct arity_increasing<ph<I>&, Rest...> {
   static const bool value = true;
};

#endif//_ARITY_INCREASING_HPP_