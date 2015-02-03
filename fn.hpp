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
template<typename F, typename T>
struct function_traits<fn_t<F, T>> {
   using Ret = typename function_traits<F>::Ret;
   static const int arity = arity<T>::value;
   static const bool is_function = true;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct arity;
template<typename P, typename ...Rest>
struct arity<std::tuple<P, Rest...>> {
   static const int value = arity<std::tuple<Rest...>>::value;
};
template<int I, typename ...Rest>
struct arity<std::tuple<ph<I>, Rest...>> {
   static const int rest_arity =arity<std::tuple<Rest...>>::value;
   static const int value = (I > rest_arity) ? I :rest_arity;
};
template<int I, typename F, typename ...Rest>
struct arity<std::tuple<std::tuple<int_list<I>, F>, Rest...>> {
   static const int this_arity = I +function_traits<F>::arity;
   static const int rest_arity =arity<std::tuple<Rest...>>::value;
   static const int value = (this_arity >rest_arity) ? this_arity : rest_arity;
};
template<>
struct arity<std::tuple<>> {
   static const int value = 0;
};
template<typename ...A>
struct arity_increasing;
template<typename A, typename ...Rest>
struct arity_increasing<A, Rest...> {
   static const bool value = (function_traits<A>::arity > 0) ||arity_increasing<Rest...>::value;
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
   static const bool value = !arity_increasing<A...>::value && (arity<T>::value == sizeof...(A));
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<int ArgIndex, int ArgsCount,typename Item, typename A, typename ...Rest>
auto get_conditionally(Item&& item, A&&, Rest&&... rest)
-> decltype(get_conditionally<ArgIndex + 1, ArgsCount>(std::forward<Item>(item),std::forward<Rest>(rest)...)) {
     return get_conditionally<ArgIndex + 1, ArgsCount>(std::forward<Item>(item), std::forward<Rest>(rest)...);
}
template<int ArgIndex, int ArgsCount,typename Item>
auto get_conditionally(Item&& item)
-> decltype(item) {
     return item;
}
template<int ArgIndex, int ArgsCount, int I>
ph<I-ArgsCount> get_conditionally(ph<I>) {
   return ph<I-ArgsCount>();
}
template<int ArgIndex, typename A>
auto decorate_arg(int_list<0>, A&& a)
-> decltype(a) {
     return a;
}
template<int ArgIndex, typename A>
std::tuple<int_list<ArgIndex>,std::remove_reference_t<A>>decorate_arg(int_list<1>, A&& a) {
   return std::make_tuple(int_list<ArgIndex>(), a);
}
template<int ArgIndex, int ArgsCount,typename A, typename ...Rest>
auto get_conditionally(ph<ArgIndex>, A&& a, Rest&&...)
-> decltype(decorate_arg<ArgIndex>(int_list<function_traits<std::remove_reference_t<A>>::is_function>(), std::forward<A>(a))) {
     return decorate_arg<ArgIndex>(int_list<function_traits<std::remove_reference_t<A>>::is_function>(), std::forward<A>(a));
}
template<typename T, int ...I, typename ...A>
auto apply_arguments(T&& t, int_list<I...>, A&&... a)
-> decltype(std::make_tuple(get_conditionally<1, sizeof...(A)>(std::get<I>(t), /*std::forward<A>(a) - bug in VC?*/a...)...)) {
     return std::make_tuple(get_conditionally<1,sizeof...(A)>(std::get<I>(t), /*std::forward<A>(a) - bug in VC?*/a...)...);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename A>
A get_value(A&& a) {
   return a;
}
template<int I, typename F, typename T>
auto get_value(std::tuple<int_list<I>,fn_t<F, T>> a)
-> decltype(std::get<1>(a)()) {
   return std::get<1>(a)();
}
template<typename F, typename T, int ...I>
auto apply_tuple_to_function_impl(F&& f, T&& t, int_list<I...>)
-> decltype(f(get_value(std::get<I>(t))...)) {
     return f(get_value(std::get<I>(t))...);
}
template<typename F, typename T>
auto apply_tuple_to_function(F&& f, T&& t)
-> decltype(apply_tuple_to_function_impl(std::forward<F>(f), std::forward<T>(t), indices_for<T>())) {
     return apply_tuple_to_function_impl(std::forward<F>(f), std::forward<T>(t), indices_for<T>());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F, int I, typename T>
fn_t<F, T> invoke(F&& f, int_list<I>, T&& t) {
   return fn_t<F, T>(f, t);
};
template<typename F, typename T>
auto invoke(F&& f, int_list<1>, T&& t)
-> decltype(apply_tuple_to_function(std::forward<F>(f), std::forward<T>(t))) {
     return apply_tuple_to_function(std::forward<F>(f), std::forward<T>(t));
};
template<typename F, typename T, typename ...A>
auto process(F&& f, T&& t, A&& ...a)
-> decltype(invoke(std::forward<F>(f),int_list<arity_matches<T, A...>::value>(),apply_arguments(std::forward<T>(t),indices_for<T>(), std::forward<A>(a)...))) {
     return invoke(std::forward<F>(f),int_list<arity_matches<T, A...>::value>(),apply_arguments(std::forward<T>(t),indices_for<T>(), std::forward<A>(a)...));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F, typename T>
struct fn_t {
   fn_t(F f) : _f(f) {}
   fn_t(F f, T t) : _f(f), _t(t) {}
   template<typename ...A>
   auto operator()(A&&... a)
      -> decltype(process(_f, std::forward<T>(_t), std::forward<A>(a)...)) {
           return process(_f, std::forward<T>(_t), std::forward<A>(a)...);
   }
private:
   F _f;
   T _t;
};
template<typename F>
auto fn(F f)
-> decltype(fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(f)) {
     return fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(f);
}
template<typename F, typename ...A>
auto fn(F f, A&& ...a)
-> decltype(fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(f)(std::forward<A>(a)...)) {
     return fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(f)(std::forward<A>(a)...);
}
#endif//_fn_hpp_
