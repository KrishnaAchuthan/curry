#ifndef _IMMUTABLE_LIST_HPP_
#define _IMMUTABLE_LIST_HPP_

#include "..\arity\function_traits.hpp"
#include "..\util\fn_fwd.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <initializer_list>
#include <memory>

namespace curry {

template<typename T>
struct immutable_list {
   struct immutable_list_item {
      immutable_list_item(T v, std::shared_ptr<const immutable_list_item> t) : _val(v), _next(t) {}
      T _val;
      std::shared_ptr<const immutable_list_item> _next;
   };
   friend immutable_list_item;
   explicit immutable_list(std::shared_ptr<const immutable_list_item> immutable_list_items) : _head(immutable_list_items) {}

   immutable_list() : _head(nullptr) {}
   immutable_list(T v, immutable_list t) : _head(new immutable_list_item(v, t._head)) {}
   immutable_list(std::initializer_list<T> init) : _head(nullptr) {
      for (auto it = std::rbegin(init); it != std::rend(init); ++it) {
         _head = std::shared_ptr<const immutable_list_item>(new immutable_list_item(*it, _head));
      }
   }

   std::shared_ptr<const immutable_list_item> _head;
};

template<typename T>
auto make_list() {
   return immutable_list<T>();
}

template<typename T>
auto make_list(std::initializer_list<T> init) {
   return immutable_list<T>(init);
}

struct empty_impl {
   template<typename T>
   auto operator()(immutable_list<T> lst) const {
      return !lst._head.get();
   }
};
template<>
struct function_traits<empty_impl> {
   static const bool is_function = true;
   static const int arity = 1;
}; 
auto empty = fn(empty_impl());

struct head_impl {
   template<typename T>
   auto operator()(immutable_list<T> lst) const {
      assert(!empty(lst));
      return (lst._head)->_val;
   }
};
template<>
struct function_traits<head_impl> {
   static const bool is_function = true;
   static const int arity = 1;
};
auto head = fn(head_impl());

struct tail_impl {
   template<typename T>
   auto operator()(immutable_list<T> lst) const {
      assert(!empty(lst));
      return immutable_list<T>(lst._head->_next);
   }
};
template<>
struct function_traits<tail_impl> {
   static const bool is_function = true;
   static const int arity = 1;
};
auto tail = fn(tail_impl());

template<typename T>
auto operator+(T v, immutable_list<T> lst) {
   return immutable_list<T>(std::move(v), lst);
}

template<class T>
auto operator+(immutable_list<T> a, immutable_list<T> b) {
   if (empty(a)) {
      return b;
   }
   return immutable_list<T>(head(a), tail(a) + b);
}

struct fmap_impl {
   template<class T, class F>
   auto operator()(F f, immutable_list<T> lst) const 
   -> immutable_list<decltype(f(std::declval<T>()))> {
      using U = decltype(f(std::declval<T>()));
      static_assert(std::is_convertible<F, std::function<U(T)>>::value, "fmap requires a function type U(T)");
      if (empty(lst)) {
         return immutable_list<U>();
      }
      else {
         return immutable_list<U>(f(head(lst)), fmap_impl()(f, tail(lst)));
      }
   }
};
template<>
struct function_traits<fmap_impl> {
   static const bool is_function = true;
   static const int arity = 2;
};
auto fmap = fn(fmap_impl());

struct filter_impl {
   template<class T, class P>
   immutable_list<T> operator()(P p, immutable_list<T> lst) const {
      using U = decltype(f(std::declval<T>()));
      static_assert(std::is_convertible<F, std::function<U(T)>>::value, "filter requires a function type U(T)");
      if (empty(lst)) {
         return immutable_list<T>();
      }
      if (p(head(lst))) {
         return immutable_list<T>(head(lst), filter_impl()(p, tail(lst)));
      }
      else {
         return filter(p, tail(lst));
      }
   }
};
template<>
struct function_traits<filter_impl> {
   static const bool is_function = true;
   static const int arity = 2;
};
auto filter = fn(filter_impl());

struct foldr_impl {
   template<class T, class U, class F>
   U operator()(F f, U acc, immutable_list<T> lst) const {
      static_assert(std::is_convertible<F, std::function<U(T)>>::value, "foldr requires a function type U(T)");
      if (empty(lst)) {
         return acc;
      }
      else {
         return f(head(lst), foldr(f, acc, tail(lst)));
      }
   }
};
template<>
struct function_traits<foldr_impl> {
   static const bool is_function = true;
   static const int arity = 3;
};
auto foldr = fn(foldr_impl());

struct foldl_impl {
   template<class T, class U, class F>
   U operator()(F f, U acc, immutable_list<T> lst) const {
      static_assert(std::is_convertible<F, std::function<U(T)>>::value, "foldl requires a function type U(T)");
      if (empty(lst)) {
         return acc;
      }
      else {
         return foldl_impl()(f, f(acc, head(lst)), tail(lst));
      }
   }
};
template<>
struct function_traits<foldl_impl> {
   static const bool is_function = true;
   static const int arity = 3;
};
auto foldl = fn(foldl_impl());

struct for_each_impl {
   template<class T, class F>
   void operator()(immutable_list<T> lst, F f) const {
      using U = decltype(f(std::declval<T>()));
      static_assert(std::is_convertible<F, std::function<U(T)>>::value, "foldl requires a function type U(T)");
      if (!empty(lst)) {
         f(head(lst));
         for_each_impl()(tail(lst), f);
      }
   }
};
template<>
struct function_traits<for_each_impl> {
   static const bool is_function = true;
   static const int arity = 2;
};
auto for_each = fn(for_each_impl());

struct show_impl {
   template<class T>
   void operator()(immutable_list<T> lst) const {
      for_each(lst, [](T v) {
         std::cout << "(" << v << ") ";
      });
      std::cout << std::endl;
   }
};
template<>
struct function_traits<show_impl> {
   static const bool is_function = true;
   static const int arity = 1;
};
auto show = fn(show_impl());

}

#endif//_IMMUTABLE_LIST_HPP_
