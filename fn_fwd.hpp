#ifndef _FN_FWD_HPP_
#define _FN_FWD_HPP_

#ifdef _MSC_FULL_VER 
#define ConstExpr 
#else
#define ConstExpr constexpr
#endif

template<typename F, typename T>
struct fn_t;

template<typename F>
ConstExpr decltype(auto) fn(F&& f);

template<typename F, typename A, typename ...Rest>
ConstExpr decltype(auto) fn(F&& f, A&& a, Rest&& ...rest);

template<typename R, typename T, typename ...A>
ConstExpr decltype(auto) fn(R T::* const f, A&& ...a);

template<typename F>
struct function_traits;

template<typename R, typename T, typename M>
struct pointer_to_member;

template<typename F>
struct cfn_t;

#endif//_FN_FWD_HPP_