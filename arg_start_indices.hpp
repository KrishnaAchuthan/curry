#ifndef _ARG_START_INDICES_HPP_
#define _ARG_START_INDICES_HPP_

#include "tuple_arity.hpp"
#include "int_list.hpp"
#include <tuple>

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
struct arg_start_indices 
     : arg_start_indices_impl<std::tuple<>, 0, T> {
};

#endif//_ARG_START_INDICES_HPP_
