#ifndef _MAYBE_HPP_
#define _MAYBE_HPP_

#include "..\monads\isabox.hpp"
#include <vector>

template<typename T>
struct maybe_t {
   maybe_t() : _value(), _is_valid(false) {
   }
   maybe_t(T&& value) : _value(std::move(value)), _is_valid(true) {
   }
   bool is_valid() const { return _is_valid; }
   const T& get() const& { return _value; }
   T& get() & { return _value; }
   T get() && { return std::move(_value); }
   T _value;
   bool _is_valid;
};

template<typename T>
struct maybe_t<const T&> {
   maybe_t() : _value(nullptr), _is_valid(false) {
   }
   maybe_t(const T& value) : _value(&value), _is_valid(true) {
   }
   maybe_t(const T* value) : _value(value), _is_valid(value ? true : false) {
   }
   bool is_valid() const { return _is_valid; }
   const T& get() const { return *_value; }
   const T* _value;
   bool _is_valid;
};

template<typename T>
struct maybe_t<T&> {
   maybe_t() : _value(nullptr), _is_valid(false) {
   }
   maybe_t(T& value) : _value(&value), _is_valid(true) {
   }
   maybe_t(T* value) : _value(value), _is_valid(value ? true : false) {
   }
   bool is_valid() const { return _is_valid; }
   T& get() { return *_value; }
   const T& get() const { return *_value; }
   T* _value;
   bool _is_valid;
};

template<typename T>
struct isabox<maybe_t<T>> : std::true_type{
   template<typename OtherT>
   struct box_map_value_t {
      using type = OtherT;
   };

   template<typename OtherT>
   struct box_map_value_t<maybe_t<OtherT>> {
      using type = typename box_map_value_t<OtherT>::type;
   };
   template<typename Item, typename F>
   static auto box_map_impl(const Item& item, F f) {
      f(item);
      return true;
   }

   template<typename OtherT, typename F>
   static auto box_map_impl(const maybe_t<OtherT>& m, F f) {
      if (m.is_valid() && box_map_impl(m.get(), [&f](auto&& item) { f(item); })) {
         return true;
      }
      return false;
   }

   template<typename F>
   static auto box_map(const maybe_t<T>& m, F f) {
      using ResultT = typename box_map_value_t<T>::type;
      if (m.is_valid()) {
         ResultT result;
         if (box_map_impl(f(m.get()), [&result](auto&& item) { result = item; })) {
            return maybe(std::move(result));
         }
      }
      return maybe<ResultT>();
   }
};

template<typename T>
auto maybe() {
   return maybe_t<T>();
}

template<typename T>
auto maybe(T& value) {
   return maybe_t<T&>(value);
}

template<typename T>
auto maybe(T* value) {
   return maybe_t<T*>(value);
}

template<typename T>
auto maybe(T&& value) {
   return maybe_t<T>(std::move(value));
}

#endif//_MAYBE_HPP_