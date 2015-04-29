#ifndef _IMMUTABLE_LIST_HPP_
#define _IMMUTABLE_LIST_HPP_

#include <cassert>
#include <functional>
#include <iostream>
#include <initializer_list>
#include <memory>

template<typename T>
struct immutable_list {
   struct immutable_list_item {
      immutable_list_item(T v, std::shared_ptr<const immutable_list_item> tail) : _val(v), _next(tail) {}
      T _val;
      std::shared_ptr<const immutable_list_item> _next;
   };
   friend immutable_list_item;
   explicit immutable_list(std::shared_ptr<const immutable_list_item> immutable_list_items) : _head(immutable_list_items) {}

   immutable_list() : _head(nullptr) {}
   immutable_list(T v, immutable_list tail) : _head(new immutable_list_item(v, tail._head)) {}
   immutable_list(std::initializer_list<T> init) : _head(nullptr) {
      for (auto it = std::rbegin(init); it != std::rend(init); ++it) {
         _head = new immutable_list_item(*it, _head);
      }
   }

   std::shared_ptr<const immutable_list_item> _head;
};

template<typename T>
auto make_list() {
   return immutable_list<T>();
}

template<typename T>
auto empty(immutable_list<T> lst) {
   return !lst._head.get();
}

template<typename T>
auto head(immutable_list<T> lst) {
   assert(!empty(lst));
   return (lst._head)->_val;
}

template<typename T>
auto tail(immutable_list<T> lst) {
   assert(!empty(lst));
   return immutable_list<T>(lst._head->_next);
}

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

template<class T, class F>
auto fmap(F f, immutable_list<T> lst) {
   using U = decltype(f(std::declval<T>()));
   static_assert(std::is_convertible<F, std::function<U(T)>>::value, "fmap requires a function type U(T)");
   if (empty(lst)) {
      return immutable_list<U>();
   }
   else {
      return immutable_list<U>(f(head(lst)), fmap<U>(f, tail(lst)));
   }
}

template<class T, class P>
immutable_list<T> filter(P p, immutable_list<T> lst) {
   using U = decltype(f(std::declval<T>()));
   static_assert(std::is_convertible<F, std::function<U(T)>>::value, "filter requires a function type U(T)");
   if (empty(lst)) {
      return immutable_list<T>();
   }
   if (p(head(lst))) {
      return immutable_list<T>(head(lst), filter(p, tail(lst)));
   }
   else {
      return filter(p, tail(lst));
   }
}

template<class T, class U, class F>
auto foldr(F f, U acc, immutable_list<T> lst) {
   using U = decltype(f(std::declval<T>()));
   static_assert(std::is_convertible<F, std::function<U(T)>>::value, "foldr requires a function type U(T)");
   if (empty(lst)) {
      return acc;
   }
   else {
      return f(head(lst), foldr(f, acc, tail(lst)));
   }
}

template<class T, class U, class F>
auto foldl(F f, U acc, immutable_list<T> lst) {
   using U = decltype(f(std::declval<T>()));
   static_assert(std::is_convertible<F, std::function<U(T)>>::value, "foldl requires a function type U(T)");
   if (empty(lst)) {
      return acc;
   }
   else {
      return foldl(f, f(acc, head(lst)), tail(lst));
   }
}

template<class T, class F>
auto for_each(immutable_list<T> lst, F f) {
   using U = decltype(f(std::declval<T>()));
   static_assert(std::is_convertible<F, std::function<U(T)>>::value, "foldl requires a function type U(T)");
   if (empty(lst)) {
      f(head(lst));
      for_each(tail(lst), f);
   }
}

template<class T>
auto show(immutable_list<T> lst) {
   for_each(lst, [](T v) {
      std::cout << "(" << v << ") ";
   });
   std::cout << std::endl;
}

#endif//_IMMUTABLE_LIST_HPP_

