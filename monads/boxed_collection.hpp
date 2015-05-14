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

   template<typename OtherT, typename OtherAlloc>
   struct box_map_value_t<std::vector<OtherT, OtherAlloc>> {
      using type = typename box_map_value_t<OtherT>::type;
   };

   template<typename Item, typename F>
   static void box_map_impl(const Item& item, F f) {
      f(item);
   }

   template<typename Item, typename F>
   static void box_map_impl(Item&& item, F f) {
      f(std::move(item));
   }

   template<typename C, typename I>
   static void add_item(C& c, const I& i) {
      c.push_back(i);
   }
   template<typename C, typename I>
   static void add_item(C& c, I&& i) {
      c.push_back(std::move(i));
   }
   template<typename C, typename I>
   static void add_item(C& c, const std::vector<I>& v) {
      for (auto& i : v)
         c.push_back(i);
   }
   template<typename C, typename I>
   static void add_item(C& c, std::vector<I>&& v) {
      for (auto&& i : std::move(v))
         c.push_back(std::move(i));
   }

   template<typename OtherT, typename F>
   static void box_map_impl(const boxed_vector<OtherT>& collection, F f) {
      //using new_collection_type = std::vector<typename box_map_value_t<typename std::decay_t<OtherT>::value_type>::type>; //(std::vector<decltype(f(*collection._v.begin())), Alloc>;
      //using new_collection_type = std::vector<decltype(f(*collection._v.begin()))>;
      using new_collection_type = std::vector<typename box_map_value_t<decltype(f(std::declval<typename T::value_type>()))>::type>;
      using return_type = boxed_vector<new_collection_type>;
      new_collection_type ret;
      for (auto&& item : collection._v) {
         box_map_impl(item, [&ret](auto&& item) {add_item(ret, item); });
      }
      for (auto&& item : ret) {
         f(item); //why not directly call instead of an intermediate vector?
      }
   }

   template<typename OtherT, typename F>
   static void box_map_impl(boxed_vector<OtherT>&& collection, F f) {
      //using new_collection_type = std::vector<typename box_map_value_t<typename std::decay_t<OtherT>::value_type>::type>; //(std::vector<decltype(f(*collection._v.begin())), Alloc>;
      //using new_collection_type = std::vector<decltype(f(*collection._v.begin()))>;
      using new_collection_type = std::vector<typename box_map_value_t<decltype(f(std::declval<typename T::value_type>()))>::type>;
      using return_type = boxed_vector<new_collection_type>;
      new_collection_type ret;
      for (auto&& item : std::move(collection)._v) {
         box_map_impl(item, [&ret](auto&& item) {add_item(ret, std::move(item)); });
      }
      for (auto&& item : ret) {
         f(std::move(item)); //why not directly call instead of an intermediate vector?
      }
   }

   template<typename F>
   static auto box_map(const boxed_vector<T>& collection, F f) {
      //using new_collection_type = std::vector<typename box_map_value_t<value_type>::type>; //(std::vector<decltype(f(*collection._v.begin())), Alloc>;
      //using new_collection_type = std::vector<decltype(f(*collection._v.begin()))>;
      //using new_collection_type = std::vector<decltype(f(std::declval<typename T::value_type>()))>;
      using new_collection_type = std::vector<typename box_map_value_t<decltype(f(std::declval<typename T::value_type>()))>::type>;
      using return_type = boxed_vector<new_collection_type>;
      new_collection_type ret;
      for (auto&& item : collection._v) {
         box_map_impl(f(item), [&ret](auto&& item) {add_item(ret, item); });
      }
      return return_type(std::move(ret));
   }
   template<typename F>
   static auto box_map(boxed_vector<T>&& collection, F f) {
      //using new_collection_type = std::vector<typename box_map_value_t<value_type>::type>; //(std::vector<decltype(f(*collection._v.begin())), Alloc>;
      //using new_collection_type = std::vector<decltype(f(*collection._v.begin()))>;
      using new_collection_type = std::vector<typename box_map_value_t<decltype(f(std::declval<typename T::value_type>()))>::type>;
      using return_type = boxed_vector<new_collection_type>;
      new_collection_type ret;
      for (auto&& item : collection._v) {
         box_map_impl(f(std::move(item)), [&ret](auto&& item) {add_item(ret, std::move(item));});
      }
      return return_type(std::move(ret));
   }
};

struct with_each_t {
   template<typename T>
   auto operator()(T& v) {
      return boxed_vector<T&>(v);
   }
   template<typename T>
   auto operator()(T&& v) {
      return boxed_vector<T>(std::move(v));
   }
};
template<>
struct function_traits<with_each_t> {
   static const bool is_function = true;
   static const int arity = 1;
};
auto with_each = fn(with_each_t());

template<typename Box>
auto unbox(Box b) {
   return b._v;
}

}

#endif//_BOXED_COLLECTION_HPP_