#ifndef _FN_FWD_HPP_
#define _FN_FWD_HPP_

template<typename F, typename T>
struct fn_t;

template<typename F>
auto fn(F&& f);

template<typename F, typename ...A>
auto fn(F&& f, A&& ...a);

template<typename R, typename T, typename ...A>
auto fn(R T::* const f, A&& ...a);

template<typename F>
struct function_traits;

template<typename R, typename T, typename M>
struct pointer_to_member;

#endif//_FN_FWD_HPP_