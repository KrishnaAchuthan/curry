#ifndef _BOXED_COLLECTION_HPP_
#define _BOXED_COLLECTION_HPP_

#include "isabox.hpp"
#include <vector>

template<typename Vector>
struct boxed_vector;

template<typename T, typename Alloc>
struct boxed_vector<std::vector<T, Alloc>> {
   using collection_type = std::vector<T, Alloc>;
   boxed_vector() : _v() {
   }
   boxed_vector(collection_type&& v) : _v(std::move(v)) {
   }
   boxed_vector(const collection_type& v) : _v(v) {
   }
   collection_type _v;
};

template<typename T, typename Alloc>
struct isabox<boxed_vector<std::vector<T, Alloc>>> : std::true_type {
   using collection_type = std::vector<T, Alloc>;
   using value_type = T;

   template<typename T>
   struct box_map_value_t {
      using type = T;
   };

   template<typename OtherT, typename OtherAlloc>
   struct box_map_value_t<boxed_vector<std::vector<OtherT, OtherAlloc>>> {
      using type = typename box_map_value_t<OtherT>::type;
   };

   template<typename Item, typename F>
   static void box_map_impl(const Item& item, F f) {
      f(item);
   }

   template<typename F>
   static void box_map_impl(const boxed_vector<collection_type>& collection, F f) {
      using new_collection_type = std::vector<typename box_map_value_t<T>::type>; //(std::vector<decltype(f(*collection._v.begin())), Alloc>;
      using return_type = boxed_vector<new_collection_type>;
      return_type ret;
      for (auto&& item : collection._v) {
         box_map_impl(item, [&ret](auto&& item) {ret._v.push_back(item); });
      }
      for (auto&& item : ret._v) {
         f(item);
      }
   }

   template<typename F>
   static auto box_map(const boxed_vector<collection_type>& collection, F f) {
      using new_collection_type = std::vector<typename box_map_value_t<T>::type>; //(std::vector<decltype(f(*collection._v.begin())), Alloc>;
      using return_type = boxed_vector<new_collection_type>;
      return_type ret;
      for (auto&& item : collection._v) {
         box_map_impl(f(item), [&ret](auto&& item) {ret._v.push_back(item); });
      }
      return ret;
   }
};

template<typename T, typename Alloc>
auto with_each(const std::vector<T, Alloc>& v) {
   return boxed_vector<std::vector<T, Alloc>>(v);
}

template<typename T, typename Alloc>
auto with_each(std::vector<T, Alloc>&& v) {
   return boxed_vector<std::vector<T, Alloc>>(std::move(v));
}

#endif//_BOXED_COLLECTION_HPP_