#ifndef _BOXED_COLLECTION_HPP_
#define _BOXED_COLLECTION_HPP_

#include "..\monads\isabox.hpp"
#include <vector>

namespace curry {

template<typename Vector>
struct boxed_vector;

template<typename T, typename Alloc>
struct boxed_vector<const std::vector<T, Alloc>&> {
   using collection_type = std::vector<T, Alloc>;
   boxed_vector(const collection_type& v) : _v(v) {
   }
   const collection_type& _v;
};

template<typename T, typename Alloc>
struct boxed_vector<std::vector<T, Alloc>&> {
   using collection_type = std::vector<T, Alloc>;
   boxed_vector(collection_type& v) : _v(v) {
   }
   collection_type& _v;
};

template<typename T, typename Alloc>
struct boxed_vector<std::vector<T, Alloc>> {
   using collection_type = std::vector<T, Alloc>;
   boxed_vector() : _v() {
   }
   boxed_vector(collection_type&& v) : _v(std::move(v)) {
   }
   collection_type _v;
};

template<typename T>
struct isabox<boxed_vector<T>> : std::true_type {
   using collection_type = std::decay_t<T>;
   using value_type = typename collection_type::value_type;

   template<typename OtherT>
   struct box_map_value_t {
      using type = OtherT;
   };

   template<typename OtherT, typename OtherAlloc>
   struct box_map_value_t<boxed_vector<std::vector<OtherT, OtherAlloc>>> {
      using type = typename box_map_value_t<OtherT>::type;
   };

   template<typename Item, typename F>
   static void box_map_impl(const Item& item, F f) {
      f(item);
   }

   template<typename OtherT, typename F>
   static void box_map_impl(const boxed_vector<OtherT>& collection, F f) {
      using new_collection_type = std::vector<typename box_map_value_t<typename std::decay_t<OtherT>::value_type>::type>; //(std::vector<decltype(f(*collection._v.begin())), Alloc>;
      using return_type = boxed_vector<new_collection_type>;
      new_collection_type ret;
      for (auto&& item : collection._v) {
         box_map_impl(item, [&ret](auto&& item) {ret.push_back(item); });
      }
      for (auto&& item : ret) {
         f(item); //why not directly call instead of an intermediate vector?
      }
   }

   template<typename F>
   static auto box_map(const boxed_vector<T>& collection, F f) {
      using new_collection_type = std::vector<typename box_map_value_t<value_type>::type>; //(std::vector<decltype(f(*collection._v.begin())), Alloc>;
      using return_type = boxed_vector<new_collection_type>;
      new_collection_type ret;
      for (auto&& item : collection._v) {
         box_map_impl(f(item), [&ret](auto&& item) {ret.push_back(item); });
      }
      return return_type(std::move(ret));
   }
};

template<typename T>
auto with_each(T& v) {
   return boxed_vector<T&>(v);
}

template<typename T>
auto with_each(T&& v) {
   return boxed_vector<T>(std::move(v));
}

}

#endif//_BOXED_COLLECTION_HPP_