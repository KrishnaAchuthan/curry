#ifndef _FN_FWD_HPP_
#define _FN_FWD_HPP_

#ifdef _MSC_FULL_VER 
#define constexpr 
#endif

template<typename F, typename T>
struct fn_t;

template<typename F>
constexpr decltype(auto) fn(F&& f);

template<typename F, typename A, typename ...Rest>
constexpr decltype(auto) fn(F&& f, A&& a, Rest&& ...rest);

template<typename R, typename T, typename ...A>
constexpr decltype(auto) fn(R T::* const f, A&& ...a);

template<typename F>
struct function_traits;

template<typename R, typename T, typename M>
struct pointer_to_member;

#endif//_FN_FWD_HPP_