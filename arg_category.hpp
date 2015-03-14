#ifndef _ARG_CATEGORY_HPP_
#define _ARG_CATEGORY_HPP_

#include "fn_fwd.hpp"
#include "int_list.hpp"
#include "placeholder.hpp"
#include <tuple>

enum class ArgCategory { 
	Regular, 
	Placeholder, 
	AppliedFunction, 
	Function 
};

template<typename T>
struct arg_category_impl {
	static const ArgCategory value = ArgCategory::Regular;
};

template<int I>
struct arg_category_impl<ph<I>> {
	static const ArgCategory value = ArgCategory::Placeholder;
};

template<int I, typename F, typename ...A>
struct arg_category_impl<std::tuple<int_list<I>, F, A...>> {
	static const ArgCategory value = ArgCategory::AppliedFunction;
};

template<typename F, typename T>
struct arg_category_impl<fn_t<F, T>> {
	static const ArgCategory value = ArgCategory::Function;
};

template<typename T>
using arg_category = 
      std::integral_constant<ArgCategory, arg_category_impl<typename std::decay<T>::type>::value>;

#endif//_ARG_CATEGORY_HPP_