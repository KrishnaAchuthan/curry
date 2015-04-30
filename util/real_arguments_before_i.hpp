#ifndef _REAL_ARGUMENTS_BEFORE_I_HPP_
#define _REAL_ARGUMENTS_BEFORE_I_HPP_

#include "placeholders\placeholder.hpp"
#include <tuple>

template<typename T, int I, int CurrentIndex>
struct real_arguments_before_I_impl;

template<typename T, typename ...Rest, int I, int CurrentIndex>
struct real_arguments_before_I_impl<std::tuple<T, Rest...>, I, CurrentIndex> {
   static const int value = 1 + real_arguments_before_I_impl<std::tuple<Rest...>, I, CurrentIndex + 1>::value;
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

#endif//_REAL_ARGUMENTS_BEFORE_I_HPP_
