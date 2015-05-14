#ifndef _FN_HPP_
#define _FN_HPP_

#include "fn\process.hpp"
#include "arity\function_traits.hpp"
#include "util\fn_fwd.hpp"
#include "util\arg_positions.hpp"
#include "util\pointer_to_member.hpp"
#include "composition\wrap_fn.hpp"
#include <tuple>
#include <type_traits>

namespace curry {

template<typename F, typename T>
struct fn_t {

   fn_t(F&& f) 
   : _f(std::move(f)) {
   }

   fn_t(F&& f, T&& t) 
   : _f(std::move(f)), _t(std::move(t)) {
   }

   template<typename ...A>
   CONSTEXPR decltype(auto) operator()(A&&... a) const& {
      return process(F(_f), T(_t), std::forward<A>(a)...);
   }

   template<typename ...A>
   decltype(auto) operator()(A&&... a) && {
      return process(std::forward<F>(_f), std::forward<T>(_t), std::forward<A>(a)...);
   }

   template<typename ...A>
   CONSTEXPR decltype(auto) compose(A&&... a) const& {
      return process(F(_f), T(_t), wrap_fn(std::forward<A>(a))...);
   }

   template<typename ...A>
   decltype(auto) compose(A&&... a) && {
      return process(std::forward<F>(_f), std::forward<T>(_t), wrap_fn(std::forward<A>(a))...);
   }

private:
   F _f;
   T _t;
};

template<typename F>
CONSTEXPR decltype(auto) fn(F&& f) {
   return fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(std::forward<F>(f));
}

template<unsigned int Arity, typename F>
CONSTEXPR decltype(auto) fn(F&& f) {
   return fn_t<F, typename arg_positions<Arity>::type>(std::forward<F>(f));
}

template<typename F, typename A, typename ...Rest>
CONSTEXPR decltype(auto) fn(F&& f, A&& a, Rest&& ...rest) {
   return fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(std::forward<F>(f))(std::forward<A>(a), std::forward<Rest>(rest)...);
}

template<typename R, typename T, typename ...A>
CONSTEXPR decltype(auto) fn(R T::* const f, A&& ...a) {
   using F = pointer_to_member<R, T, R T::*>;
   return fn(F(f), std::forward<A>(a)...);
}

template<typename F>
struct cfn_t {
   cfn_t(F&& f) : _f(std::forward<F>(f)) {
   }
   F&& _f;
};

}

#endif//_FN_HPP_
