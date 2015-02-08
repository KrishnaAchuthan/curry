#ifndef _fn_hpp_
#define _fn_hpp_

#include <tuple>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int>
struct ph {};
ph<1> _1;
ph<2> _2;
ph<3> _3;
ph<4> _4;
ph<5> _5;
ph<6> _6;
ph<7> _7;
ph<8> _8;
ph<9> _9;
ph<10> _10;
template<typename T>
struct is_placeholder {
	static const bool value = false;
};
template<int I>
struct is_placeholder<ph<I>> {
	static const bool value = true;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F, typename T>
struct fn_t;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int ...I>
struct int_list {};
template<typename I, int Count>
struct make_int_list_impl;
template<int ...I, int Count>
struct make_int_list_impl<int_list<I...>, Count> {
	using type = typename make_int_list_impl<int_list<Count - 1, I...>, Count - 1>::type;
};
template<int ...I>
struct make_int_list_impl<int_list<I...>, 0> {
	using type = int_list<I...>;
};
template<int Count>
struct make_int_list {
	using type = typename make_int_list_impl<int_list<>, Count>::type;
};
template<typename T>
using indices_for = typename make_int_list<std::tuple_size<T>::value>::type;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Result, int Count>
struct arg_positions_impl;
template<typename ...Result, int Count>
struct arg_positions_impl<std::tuple<Result...>, Count> {
	using type = typename arg_positions_impl<std::tuple<ph<Count>, Result...>, Count - 1>::type;
};
template<typename ...Result>
struct arg_positions_impl<std::tuple<Result...>, 0> {
	using type = std::tuple<Result...>;
};
template<int Count>
struct arg_positions {
	using type = typename arg_positions_impl<std::tuple<>, Count>::type;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct arity;
template<typename F>
struct function_traits;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct tuple_arity;
template<typename A, typename ...Rest>
struct tuple_arity<std::tuple<A, Rest...>> {
	static const int value = tuple_arity<std::tuple<Rest...>>::value;
};
template<int I, typename ...Rest>
struct tuple_arity<std::tuple<ph<I>, Rest...>> {
	static const int rest_arity = tuple_arity<std::tuple<Rest...>>::value;
	static const int value = (I > rest_arity) ? I : rest_arity;
};
//template<int I, typename F, typename ...Rest>
//struct tuple_arity<std::tuple<std::tuple<int_list<I>, F>, Rest...>> {
//	static const int this_arity = I + function_traits<F>::arity;
//	static const int rest_arity = tuple_arity<std::tuple<Rest...>>::value;
//	static const int value = (this_arity >rest_arity) ? this_arity : rest_arity;
//};
template<int I, typename F, typename ...A, typename ...Rest>
struct tuple_arity<std::tuple<std::tuple<int_list<I>, F, A...>, Rest...>> {
	static const int this_arity = (I-1) + function_traits<F>::arity - sizeof...(A);
	static const int rest_arity = tuple_arity<std::tuple<Rest...>>::value;
	static const int value = (this_arity >rest_arity) ? this_arity : rest_arity;
};
template<typename F, typename ...A, typename ...Rest>
struct tuple_arity<std::tuple<std::tuple<F, A...>, Rest...>> {
	static const int this_arity = 0;
	static const int rest_arity = tuple_arity<std::tuple<Rest...>>::value;
	static const int value = (this_arity >rest_arity) ? this_arity : rest_arity;
};
template<>
struct tuple_arity<std::tuple<>> {
	static const int value = 0;
};
template<typename ...A>
struct arity_increasing;
template<typename A, typename ...Rest>
struct arity_increasing<A, Rest...> {
	static const bool value = (function_traits<A>::arity > 0) || arity_increasing<Rest...>::value;
};
template<>
struct arity_increasing<> {
	static const bool value = false;
};
template<int I, typename ...Rest>
struct arity_increasing<ph<I>, Rest...> {
	static const bool value = true;
};
template<int I, typename ...Rest>
struct arity_increasing<ph<I>&, Rest...> {
	static const bool value = true;
};
template<typename T, typename ...A>
struct arity_matches {
	static const bool value = !arity_increasing<A...>::value && (tuple_arity<T>::value == sizeof...(A));
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F>
struct function_traits {
	static const int arity = 0;
	static const bool is_function = false;
};
template<typename R, typename ...P>
struct function_traits<R(*)(P...)> {
	using Ret = R;
	static const int arity = sizeof...(P);
	static const bool is_function = true;
};
template<typename R, typename ...P>
struct function_traits<R(&)(P...)> {
	using Ret = R;
	static const int arity = sizeof...(P);
	static const bool is_function = true;
};
template<typename F, typename T>
struct function_traits<fn_t<F, T>> {
	using Ret = typename function_traits<F>::Ret;
	static const int arity = tuple_arity<T>::value;
	static const bool is_function = true;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct arg_category_impl {
	static const int value = 1;
};
template<int I>
struct arg_category_impl<ph<I>> {
	static const int value = 2;
};
template<int I, typename F, typename ...A>
struct arg_category_impl<std::tuple<int_list<I>, F, A...>> {
	static const int value = 3;
};
template<typename F, typename T>
struct arg_category_impl<fn_t<F, T>> {
	static const int value = 4;
};
template<typename T>
struct arg_category {
	static const int value = arg_category_impl<typename std::decay<T>::type>::value;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int ArgsCount, typename Item, typename A>
auto get_conditionally(int_list<1>, Item&& item, A&&) {
	return item;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int I, int J, typename A>
auto decorate_arg(int_list<J>, A&& a) {
	return std::forward<A>(a);
}
template<int I, typename A>
auto decorate_arg(int_list<4>, A&& a) {
	return std::make_tuple(int_list<I>(), a);
}
template<int ArgsCount, int I, typename A>
auto get_conditionally_impl1(std::true_type, A&& a) {
	return decorate_arg<I>(int_list<arg_category<decltype(std::get<I - 1>(a))>::value>(), std::get<I - 1>(std::forward<A>(a)));
}
template<int ArgsCount, int I, typename A>
auto get_conditionally_impl1(std::false_type, A&&) {
	return ph<I - ArgsCount>();
}
template<int ArgsCount, int I, typename A>
auto get_conditionally(int_list<2>, ph<I> item, A&& a) {
	return get_conditionally_impl1<ArgsCount, I>(std::integral_constant<bool, (ArgsCount >= I)>(), std::forward<A>(a));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int I, int ...J, typename F, typename A>
auto add_arguments_to_function(int_list<J...>, F&& f, A&& a) {
	return std::make_tuple(f, std::get<I + J - 1>(std::forward<A>(a))...);
}
template<int ArgsCount, int I, typename F, typename A>
auto get_conditionally_impl2(std::true_type, std::tuple<int_list<I>, F>&& item, A&& a) {
	return add_arguments_to_function<I>(typename make_int_list<function_traits<typename std::remove_reference<F>::type>::arity>::type(), std::get<1>(std::forward<std::tuple<int_list<I>, F>>(item)), std::forward<A>(a));
}
template<int ArgsCount, int I, typename F, typename A>
auto get_conditionally_impl2(std::false_type, std::tuple<int_list<I>, F>&& item, A&&) {
	return std::make_tuple(int_list<I - ArgsCount>(), std::get<1>(std::forward<std::tuple<int_list<I>, F>>(item)));
}
template<int ArgsCount, int I, typename F, typename A>
auto get_conditionally(int_list<3>, std::tuple<int_list<I>, F> item, A&& a) {
	return get_conditionally_impl2<ArgsCount, I>(std::integral_constant<bool, ArgsCount >= I >(), std::forward<std::tuple<int_list<I>, F>>(item), std::forward<A>(a));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int ArgsCount, int ...I, typename T, typename A>
auto apply_arguments(int_list<I...>, T&& t, A&& a) {
	return std::make_tuple(get_conditionally<ArgsCount>(int_list<arg_category<decltype(std::get<I>(t))>::value>(), std::get<I>(std::forward<T>(t)), std::forward<A>(a))...);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename A>
auto get_value(A&& a) {
	return std::forward<A>(a);
}
template<int ...I, typename F, typename A>
auto get_value_impl(int_list<I...>, F&& f, A&& a) {
	return f(std::get<I+1>(std::forward<A>(a))...);
}
template<typename F, typename ...A>
auto get_value(std::tuple<F, A...>&& a) {
	return get_value_impl(typename make_int_list<sizeof...(A)>::type(), std::get<0>(std::forward<std::tuple<F, A...>>(a)), std::forward<std::tuple<F, A...>>(a));
}
template<typename F, typename T, int ...I>
auto apply_tuple_to_function(int_list<I...>, F&& f, T&& t) {
	return f(get_value(std::get<I>(std::forward<T>(t)))...);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F>
auto fn(F&& f);

template<typename F, typename T>
auto invoke(F&& f, std::false_type, T&& t) {
	return fn(fn_t<F, T>(std::forward<F>(f), std::forward<T>(t)));
}
template<typename F, typename T>
auto invoke(F&& f, std::true_type, T&& t) {
	return apply_tuple_to_function(indices_for<T>(), std::forward<F>(f), std::forward<T>(t));
}
template<typename F, typename T, typename ...A>
auto process(F&& f, T&& t, A&& ...a) {
	return invoke(std::forward<F>(f), std::integral_constant<bool, arity_matches<T, A...>::value>(), apply_arguments<sizeof...(A)>(indices_for<T>(), std::forward<T>(t), std::forward_as_tuple(std::forward<A>(a)...)));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F, typename T>
struct fn_t {
	fn_t(F f) : _f(f) {}
	fn_t(F f, T t) : _f(f), _t(t) {}
	template<typename ...A>
	auto operator()(A&&... a) {
		return process(std::forward<F>(_f), std::forward<T>(_t), std::forward<A>(a)...);
	}
private:
	F _f;
	T _t;
};
template<typename F>
auto fn(F&& f) {
	return fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(std::forward<F>(f));
}
template<typename F, typename ...A>
auto fn(F&& f, A&& ...a) {
	return fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(std::forward<F>(f))(std::forward<A>(a)...);
}

#endif//_fn_hpp_
