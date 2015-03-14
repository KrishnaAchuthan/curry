#ifndef _FUNCTION_OPERATOR_EXISTS_HPP_
#define _FUNCTION_OPERATOR_EXISTS_HPP_

#include <type_traits>

namespace detail {

	template<typename> 
	struct sfinae_true : std::true_type {
	};

	template<typename T> 
	static sfinae_true<decltype(&(std::declval<T>().operator()))> 
		function_operator_exists(int);

	template<typename T> 
	static std::false_type 
		function_operator_exists(long);

	template<typename T>
	struct function_operator_test : decltype(function_operator_exists<T>(0)) {
	};

}

template<typename F>
struct function_operator_exists : detail::function_operator_test<F>::type {
};

#endif//_FUNCTION_OPERATOR_EXISTS_HPP_