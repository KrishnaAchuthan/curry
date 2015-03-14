#ifndef _TUPLE_ARITY_HPP_
#define _TUPLE_ARITY_HPP_

#include "fn_fwd.hpp"
#include "placeholder.hpp"
#include <tuple>

template<typename T>
struct tuple_arity {
	static const int value = function_traits<T>::arity;
};

template<typename A, typename ...Rest>
struct tuple_arity<std::tuple<A, Rest...>> {
	static const int value = tuple_arity<std::tuple<Rest...>>::value;
};

template<int I, typename ...Rest>
struct tuple_arity<std::tuple<ph<I>, Rest...>> {
	static const int rest_arity = tuple_arity<std::tuple<Rest...>>::value;
	static const int value = (I > rest_arity) ? I : rest_arity;
};

template<int I, typename F, typename ...A, typename ...Rest>
struct tuple_arity<std::tuple<std::tuple<int_list<I>, F, A...>, Rest...>> {
	static const int this_arity = (I - 1) + function_traits<F>::arity - sizeof...(A);
	static const int rest_arity = tuple_arity<std::tuple<Rest...>>::value;
	static const int value = this_arity + rest_arity;
};

template<typename F, typename ...A, typename ...Rest>
struct tuple_arity<std::tuple<std::tuple<F, A...>, Rest...>> {
	static const int this_arity = 0;
	static const int rest_arity = tuple_arity<std::tuple<Rest...>>::value;
	static const int value = (this_arity > rest_arity) ? this_arity : rest_arity;
};

template<>
struct tuple_arity<std::tuple<>> {
	static const int value = 0;
};

#endif//_TUPLE_ARITY_HPP_
