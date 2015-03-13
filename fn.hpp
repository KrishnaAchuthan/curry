#ifndef _fn_hpp_
#define _fn_hpp_

#include <tuple>
#include <type_traits>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int>
struct ph {};
ph<0> _ ;
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
template<typename F>
struct function_traits;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	static const int this_arity = (I-1) + function_traits<F>::arity - sizeof...(A);
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
template<typename> struct sfinae_true : std::true_type {};
namespace detail {
	template<typename T> static sfinae_true<decltype(&(std::declval<T>().operator()))> function_operator_exists(int);
	template<typename T> static std::false_type                                        function_operator_exists(long);
	template<typename T>
	struct function_operator_test : decltype(function_operator_exists<T>(0)) {};
}
template<typename F>
struct function_operator_exists : detail::function_operator_test<F>::type { };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename R, bool b, int a>
struct function_traits_helper {
	using Ret = R;
	static const bool is_function = b;
	static const int arity = a;
};

template<bool is_function, typename F>
struct function_traits_impl : function_traits_helper<void, false, 0> {
};

template<typename F>
struct function_traits_impl<true, F> : function_traits_impl<true, decltype(&(std::decay_t<F>)::operator())> {
	static const int arity = function_traits_impl<true, decltype(&(std::decay_t<F>)::operator())>::arity + 1;
};

template<typename R, typename T, typename M>
struct pointer_to_member;

template<bool b, typename R, typename T, typename M>
struct function_traits_impl<b, pointer_to_member<R, T, M>> : function_traits_helper<R, true, 1> {
};

template<bool b, typename R, typename ...P>             struct function_traits_impl<b, R(*)(P...)>    : function_traits_helper<R,    true,  sizeof...(P)>{};
template<bool b, typename R, typename ...P>             struct function_traits_impl<b, R(&)(P...)>    : function_traits_helper<R,    true,  sizeof...(P)>{};
template<bool b, typename R, typename ...P, typename C> struct function_traits_impl<b, R(C::*)(P...)> : function_traits_helper<R,    true,  sizeof...(P)+1>{};
template<bool b, typename F, typename T>                struct function_traits_impl<b, fn_t<F, T>>    : function_traits_helper<typename function_traits<F>::Ret, true, tuple_arity<T>::value> {};

template<typename F> struct function_traits : function_traits_impl<function_operator_exists<F>::value, F> {
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class ArgCategory {Regular, Placeholder, AppliedFunction, Function};
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
using arg_category = std::integral_constant<ArgCategory, arg_category_impl<typename std::decay<T>::type>::value>;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int ArgsCount, typename ArgPosition, typename Item, typename A>
auto get_conditionally(std::integral_constant<ArgCategory, ArgCategory::Regular>, Item&& item, A&&) {
	return item;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int I, ArgCategory SomethingElse, typename A>
auto decorate_arg(std::integral_constant<ArgCategory, SomethingElse>, A&& a) {
	return std::forward<A>(a);
}
template<int I, typename A>
auto decorate_arg(std::integral_constant<ArgCategory, ArgCategory::Function>, A&& a) {
	return std::make_tuple(int_list<I>(), a);
}
template<int ArgsCount, int I, int ArgPosition, typename A>
auto get_conditionally_impl1(std::true_type, int_list<ArgPosition>, A&& a) {
	return decorate_arg<I>(arg_category<decltype(std::get<ArgPosition + I - 1>(a))>(), std::get<ArgPosition + I - 1>(std::forward<A>(a)));
}
template<int ArgsCount, int I, int ArgPosition, typename A>
auto get_conditionally_impl1(std::false_type, int_list<ArgPosition>, A&&) {
	return ph<I - ArgsCount>();
}
template<int ArgsCount, typename ArgPosition, int I, typename A>
auto get_conditionally(std::integral_constant<ArgCategory, ArgCategory::Placeholder>, ph<I> item, A&& a) {
	return get_conditionally_impl1<ArgsCount, I>(std::integral_constant<bool, (ArgsCount >= I)>(), ArgPosition(), std::forward<A>(a));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int I, int ArgPosition, int ...J, typename F, typename A>
auto add_arguments_to_function(int_list<ArgPosition>, int_list<J...>, F&& f, A&& a) {
	return std::make_tuple(f, std::get<ArgPosition + J>(std::forward<A>(a))...);
}
template<int ArgsCount, typename ArgPosition, int I, typename F, typename A>
auto get_conditionally_impl2(std::true_type, std::tuple<int_list<I>, F>&& item, A&& a) {
	return add_arguments_to_function<I>(ArgPosition(), typename make_int_list<function_traits<typename std::remove_reference<F>::type>::arity>::type(), std::get<1>(std::forward<std::tuple<int_list<I>, F>>(item)), std::forward<A>(a));
}
template<int ArgsCount, typename ArgPosition, int I, typename F, typename A>
auto get_conditionally_impl2(std::false_type, std::tuple<int_list<I>, F>&& item, A&&) {
	return std::make_tuple(int_list<I - ArgsCount>(), std::get<1>(std::forward<std::tuple<int_list<I>, F>>(item)));
}
template<typename Result, int RunningTotal, typename T>
struct arg_start_indices_impl;

template<typename ...Result, int RunningTotal, typename T, typename ...Rest>
struct arg_start_indices_impl<std::tuple<Result...>, RunningTotal, std::tuple<T, Rest...>> {
	static const int arg_start_index = RunningTotal;
	using type = typename arg_start_indices_impl<std::tuple<Result..., int_list<arg_start_index>>, arg_start_index, std::tuple<Rest...>>::type;
};
template<typename ...Result, int RunningTotal, int I, typename F, typename ...A, typename ...Rest>
struct arg_start_indices_impl<std::tuple<Result...>, RunningTotal, std::tuple<std::tuple<int_list<I>, F, A...>, Rest...>> {
	using T = std::tuple<int_list<I>, F, A...>;
	static const int arg_start_index = RunningTotal + tuple_arity<std::tuple<T>>::value;
	using type = typename arg_start_indices_impl<std::tuple<Result..., int_list<RunningTotal>>, arg_start_index, std::tuple<Rest...>>::type;
};
template<typename ...Result, int RunningTotal>
struct arg_start_indices_impl<std::tuple<Result...>, RunningTotal, std::tuple<>> {
	using type = std::tuple<Result...>;
};
template<typename T>
struct arg_start_indices : arg_start_indices_impl<std::tuple<>, 0, T> {
};
template<int ArgsCount, typename ArgPosition, int I, typename F, typename A>
auto get_conditionally(std::integral_constant<ArgCategory, ArgCategory::AppliedFunction>, std::tuple<int_list<I>, F> item, A&& a) {
	return get_conditionally_impl2<ArgsCount, ArgPosition, I>(std::integral_constant<bool, ArgsCount >= I >(), std::forward<std::tuple<int_list<I>, F>>(item), std::forward<A>(a));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int ArgsCount, int ...I, typename T, typename A>
auto apply_arguments(int_list<I...>, T&& t, A&& a) {
	return std::make_tuple(get_conditionally<ArgsCount, std::tuple_element_t<I, typename arg_start_indices<T>::type>>(arg_category<decltype(std::get<I>(t))>(), std::get<I>(std::forward<T>(t)), std::forward<A>(a))...);
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
decltype(auto) apply_tuple_to_function(int_list<I...>, F&& f, T&& t) {
	return f(get_value(std::get<I>(std::forward<T>(t)))...);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, int I, int CurrentIndex>
struct real_arguments_before_I_impl;

template<typename T, typename ...Rest, int I, int CurrentIndex>
struct real_arguments_before_I_impl<std::tuple<T, Rest...>, I, CurrentIndex> {
	static const int value = 1 + real_arguments_before_I_impl<std::tuple<Rest...>, I, CurrentIndex+1>::value;
};

template<int PlaceholderIndex, typename ...Rest, int I, int CurrentIndex>
struct real_arguments_before_I_impl<std::tuple<ph<PlaceholderIndex>, Rest...>, I, CurrentIndex> {
	static const int value = real_arguments_before_I_impl<std::tuple<Rest...>, I, CurrentIndex + 1>::value;
};

template<int PlaceholderIndex, typename ...Rest, int I, int CurrentIndex>
struct real_arguments_before_I_impl<std::tuple<ph<PlaceholderIndex>&, Rest...>, I, CurrentIndex> {
	static const int value = real_arguments_before_I_impl<std::tuple<Rest...>, I, CurrentIndex + 1>::value;
};

template<typename T, typename ...Rest, int I>
struct real_arguments_before_I_impl<std::tuple<T, Rest...>, I, I> {
	static const int value = 0;
};

template<int PlaceholderIndex, typename ...Rest, int I>
struct real_arguments_before_I_impl<std::tuple<ph<PlaceholderIndex>, Rest...>, I, I> {
	static const int value = 0;
};

template<int PlaceholderIndex, typename ...Rest, int I>
struct real_arguments_before_I_impl<std::tuple<ph<PlaceholderIndex>&, Rest...>, I, I> {
	static const int value = 0;
};

template<int I, int CurrentIndex>
struct real_arguments_before_I_impl<std::tuple<>, I, CurrentIndex> {
	static const int value = 0;
};

template<typename T, int I>
struct real_arguments_before_I {
	static const int value = real_arguments_before_I_impl<T, I, 0>::value;
};

template<int I, typename T, typename Item>
decltype(auto) correct_anonymous_placeholder_number(Item&& item) {
	return std::forward<Item>(item);
}
template<int I, typename T>
decltype(auto) correct_anonymous_placeholder_number(ph<0>) {
	return ph<I+1-real_arguments_before_I<T, I>::value>();
}
template<int ...I, typename T>
decltype(auto) add_anonymous_placeholder_support_impl(int_list<I...>, T&& t) {
	return std::forward_as_tuple(correct_anonymous_placeholder_number<I, T>(std::get<I>(std::forward<T>(t)))...);
}
template<typename T>
decltype(auto) add_anonymous_placeholder_support(T&& t) {
	return add_anonymous_placeholder_support_impl(indices_for<T>(), std::forward<T>(t));
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
	return invoke(std::forward<F>(f), std::integral_constant<bool, arity_matches<T, A...>::value>(), apply_arguments<sizeof...(A)>(indices_for<T>(), std::forward<T>(t), add_anonymous_placeholder_support(std::forward_as_tuple(std::forward<A>(a)...))));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F, typename T>
struct fn_t {
	fn_t(F&& f) : _f(std::move(f)) {}
	fn_t(F&& f, T&& t) : _f(std::move(f)), _t(std::move(t)) {}
	template<typename ...A>
	auto operator()(A&&... a) const& {
		return process(F(_f), T(_t), std::forward<A>(a)...);
	}
	template<typename ...A>
	auto operator()(A&&... a) && {
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

template<typename R, typename T, typename M>
struct pointer_to_member {
	pointer_to_member(const M& m) :_m(m) {}

	               auto& operator()(               T& t) const { return t.*_m; }
	const          auto& operator()(const          T& t) const { return t.*_m; }
	      volatile auto& operator()(      volatile T& t) const { return t.*_m; }
	const volatile auto& operator()(const volatile T& t) const { return t.*_m; }

private:
	M _m;
};
template<typename R, typename T, typename ...A>
auto fn(R T::* const f, A&& ...a) {
	using F = pointer_to_member<R, T, R T::*>;
	return fn(F(f), std::forward<A>(a)...);
}

#endif//_fn_hpp_
