#ifndef _FUNCTION_TRAITS_HPP_
#define _FUNCTION_TRAITS_HPP_

#include "function_operator_exists.hpp"
#include "tuple_arity.hpp"
#include "fn_fwd.hpp"
#include <type_traits>

template<typename R, bool b, int a>
struct function_traits_helper {
	using Ret = R;
	static const bool is_function = b;
	static const int arity = a;
};

template<bool is_function, typename F>
struct function_traits_impl 
	 : function_traits_helper<void, false, 0> {
};

template<typename F>
struct function_traits_impl<true, F> 
	 : function_traits_impl<true, decltype(&(std::decay_t<F>)::operator())>{
	static const int arity = function_traits_impl<true, decltype(&(std::decay_t<F>)::operator())>::arity + 1;
};

template<bool b, typename R, typename T, typename M>
struct function_traits_impl<b, pointer_to_member<R, T, M>> 
	 : function_traits_helper<R, true, 1>{
};

template<bool b, typename R, typename ...P>
struct function_traits_impl<b, R(*)(P...)> 
	 : function_traits_helper<R, true, sizeof...(P)> {
};

template<bool b, typename R, typename ...P>
struct function_traits_impl<b, R(&)(P...)> 
	 : function_traits_helper<R, true, sizeof...(P)> {
};

template<bool b, typename R, typename ...P, typename C>
struct function_traits_impl<b, R(C::*)(P...)> 
	 : function_traits_helper<R, true, sizeof...(P)+1> {
};

template<bool b, typename F, typename T>
struct function_traits_impl<b, fn_t<F, T>> 
	 : function_traits_helper<typename function_traits<F>::Ret, true, tuple_arity<T>::value> {
};

template<typename F>
struct function_traits 
	 : function_traits_impl<function_operator_exists<F>::value, F> {
};

#endif//_FUNCTION_TRAITS_HPP_
