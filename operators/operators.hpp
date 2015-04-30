#ifndef _OPERATORS_HPP_

#include "..\util\fn_fwd.hpp"
#include "..\arity\function_traits.hpp"
#include "..\placeholders\placeholder.hpp"

namespace curry {

template<typename F, typename A, typename ...Rest>
auto compose(F&& f, A&& a, Rest&&... rest) {
   return (std::forward<F>(f)).compose(std::forward<A>(a), std::forward<Rest>(rest)...);
}

template<typename F, typename typename T, typename A>
auto operator * (const fn_t<F, T>& f, A&& a) {
   return compose(f, std::forward<A>(a));
}

template<typename F, typename typename T, typename A>
auto operator * (fn_t<F, T>&& f, A&& a) {
   return compose(std::forward<fn_t<F, T>>(f), std::forward<A>(a));
}

template<typename F, typename typename T, typename A>
auto operator | (A&& a, const fn_t<F, T>& f) {
   return compose(f, std::forward<A>(a));
}

template<typename F, typename typename T, typename A>
auto operator | (A&& a, fn_t<F, T>&& f) {
   return compose(std::forward<fn_t<F, T>>(f), std::forward<A>(a));
}

template<typename F1, typename typename T1, typename F2, typename T2>
auto operator | (const fn_t<F1, T1>& f1, const fn_t<F2, T2>& f2) {
   return compose(f2, f1);
}

template<typename F>
struct op_t {
   op_t(F&& f) : _f(f) {}
   template<typename T, typename U>
   auto operator()(T&& t, U&& u) const& {
      return _f(std::forward<T>(t), std::forward<U>(u));
   }
   template<typename T, typename U>
   auto operator()(T&& t, U&& u) && {
      return std::forward<F>(_f)(std::forward<T>(t), std::forward<U>(u));
   }
   F _f;
};

template<typename F>
auto op(F&& f) {
   return op_t<F>(std::forward<F>(f));
}

template<typename F>
struct function_traits<op_t<F>> {
   using Ret = void;
   static const bool is_function = true;
   static const int arity = 2;
};

template<int I, int J>
auto operator + (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs + rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator + (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs + rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator + (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs + rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator - (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs - rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator - (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs - rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator - (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs - rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator * (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs * rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator * (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs * rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator * (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs * rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator / (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs / rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator / (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs / rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator / (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs / rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator % (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs % rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator % (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs % rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator % (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs % rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator > (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs > rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator > (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs > rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator > (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs > rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator < (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs > rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator < (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs < rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator < (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs < rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator >=(ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs >= rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator >=(ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs >= rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator >= (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs >= rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator <= (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs <= rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator <= (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs <= rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator <= (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs <= rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator == (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs == rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator == (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs == rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator == (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs == rhs; }))(lhs, rhs);
}

template<int I, int J>
auto operator != (ph<I> lhs, ph<J> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs != rhs; }))(lhs, rhs);
}

template<int I, typename Rhs>
auto operator != (ph<I> lhs, Rhs&& rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs != rhs; }))(lhs, rhs);
}

template<typename Lhs, int I>
auto operator != (Lhs&& lhs, ph<I> rhs) {
   return fn(op([](auto&& lhs, auto&& rhs) {return lhs != rhs; }))(lhs, rhs);
}

template<int I>
auto operator ! (ph<I> rhs) {
   return fn(op([](auto&& rhs) {return !rhs; }))(rhs);
}

template<int I>
auto operator ~ (ph<I> rhs) {
   return fn(op([](auto&& rhs) {return ~rhs; }))(rhs);
}

template<int I>
auto operator + (ph<I> rhs) {
   return fn(op([](auto&& rhs) {return +rhs; }))(rhs);
}

template<int I>
auto operator - (ph<I> rhs) {
   return fn(op([](auto&& rhs) {return -rhs; }))(rhs);
}

}

#endif
