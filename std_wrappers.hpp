#ifndef _STD_WRAPPERS_HPP_
#define _STD_WRAPPERS_HPP_

#include "util\fn_fwd.hpp"
#include <algorithm>

namespace curry
{
   namespace __detail
   {
      struct all_of_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::all_of(f, l, func);
         }
      };
      struct any_of_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::any_of(f, l, func);
         }
      };
      struct none_of_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::none_of(f, l, func);
         }
      };
      struct for_each_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::for_each(f, l, func);
         }
      };
      struct find_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::find(f, l, func);
         }
      };
      struct find_if_fn
      {
         template<typename I, typename T>
         decltype(auto) operator()(I f, I l, const T& val) {
            return std::find_if(f, l, vsl);
         }
      };
      struct find_if_not_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::find_if_not(f, l, func);
         }
      };
	  struct find_end_fn
	  {
		  template<typename I1, typename I2>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2, I2 l2) {
			  return std::find_end(f1, l1, f2, l2);
		  }
	  };
	  struct find_end_pred_fn
	  {
		  template<typename I1, typename I2, typename P>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2, I2 l2, P pred) {
			  return std::find_end(f1, l1, f2, l2, pred);
		  }
	  };
	  struct find_first_of_fn
	  {
		  template<typename I1, typename I2>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2, I2 l2) {
			  return std::find_first_of(f1, l1, f2, l2);
		  }
	  };
	  struct find_first_of_pred_fn
	  {
		  template<typename I1, typename I2, typename P>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2, I2 l2, P pred) {
			  return std::find_first_of(f1, l1, f2, l2, pred);
		  }
	  };
	  struct adjacent_find_fn
	  {
		  template<typename I>
		  decltype(auto) operator()(I f, I l) {
			  return std::adjacent_find(f, l);
		  }
	  };
	  struct adjacent_find_pred_fn
	  {
		  template<typename I, typename P>
		  decltype(auto) operator()(I f, I l, P pred) {
			  return std::adjacent_find(f, l, pred);
		  }
	  };
	  struct count_fn
      {
         template<typename I, typename T>
         decltype(auto) operator()(I f, I l, const T& val) {
            return std::count(f, l, val);
         }
      };
      struct count_if_fn
      {
         template<typename I, typename P>
         decltype(auto) operator()(I f, I l, P pred) {
            return std::count_if(f, l, pred);
         }
      };
	  struct mismatch_fn
	  {
		  template<typename I1, typename I2>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2) {
			  return std::mismatch(f1, l1, f2);
		  }
	  };
	  struct mismatch_pred_fn
	  {
		  template<typename I1, typename I2, typename P>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2, P pred) {
			  return std::mismatch(f1, l1, f2, pred);
		  }
	  };
	  struct equal_fn
	  {
		  template<typename I1, typename I2>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2) {
			  return std::equal(f1, l1, f2);
		  }
	  };
	  struct equal_pred_fn
	  {
		  template<typename I1, typename I2, typename P>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2, P pred) {
			  return std::equal(f1, l1, f2, pred);
		  }
	  };
	  struct is_permutation_fn
	  {
		  template<typename I1, typename I2>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2) {
			  return std::is_permutation(f1, l1, f2);
		  }
	  };
	  struct is_permutation_pred_fn
	  {
		  template<typename I1, typename I2, typename P>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2, P pred) {
			  return std::is_permutation(f1, l1, f2, pred);
		  }
	  };
	  struct search_fn
	  {
		  template<typename I1, typename I2>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2) {
			  return std::search(f1, l1, f2, l2);
		  }
	  };
	  struct search_pred_fn
	  {
		  template<typename I1, typename I2, typename P>
		  decltype(auto) operator()(I1 f1, I1 l1, I2 f2, P pred) {
			  return std::search(f1, l1, f2, l2, pred);
		  }
	  };
	  struct search_n_fn
	  {
		  template<typename I, typename Size, typename T>
		  decltype(auto) operator()(I f, I l, Size count, const T& val) {
			  return std::search_n(f, l, count, val);
		  }
	  };
	  struct search_n_pred_fn
	  {
		  template<typename I, typename Size, typename T, typename P>
		  decltype(auto) operator()(I f, I l, Size count, const T& val, P pred) {
			  return std::search_n(f, l, count, val, pred);
		  }
	  };
	  struct copy_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::copy(f, l, func);
         }
      };
      struct copy_n_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::copy_n(f, l, func);
         }
      };
      struct copy_if_fn
      {
         template<typename I, typename O, typename F>
         decltype(auto) operator()(I f, I l, O d, F func) {
            return std::copy_if(f, l, d, func);
         }
      };
      struct copy_backward_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::copy_backward(f, l, func);
         }
      };
      struct move_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::move(f, l, func);
         }
      };
      struct move_backward_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::move_backward(f, l, func);
         }
      };
      struct swap_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::swap(f, l, func);
         }
      };
      struct swap_ranges_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::swap_ranges(f, l, func);
         }
      };
      struct iter_swap_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::iter_swap(f, l, func);
         }
      };
      struct transform_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::transform(f, l, func);
         }
      };
      struct replace_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::replace(f, l, func);
         }
      };
      struct replace_if_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::replace_if(f, l, func);
         }
      };
      struct replace_copy_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::replace_copy(f, l, func);
         }
      };
      struct replace_copy_if_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::replace_copy_if(f, l, func);
         }
      };
      struct fill_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::fill(f, l, func);
         }
      };
      struct fill_n_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::fill_n(f, l, func);
         }
      };
      struct generate_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::generate(f, l, func);
         }
      };
      struct generate_n_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::generate_n(f, l, func);
         }
      };
      struct remove_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::remove(f, l, func);
         }
      };
      struct remove_if_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::remove_if(f, l, func);
         }
      };
      struct remove_copy_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::remove_copy(f, l, func);
         }
      };
      struct remove_copy_if_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::remove_copy_if(f, l, func);
         }
      };
      struct unique_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::unique(f, l, func);
         }
      };
      struct unique_copy_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::unique_copy(f, l, func);
         }
      };
      struct reverse_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::reverse(f, l, func);
         }
      };
      struct reverse_copy_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::reverse_copy(f, l, func);
         }
      };
      struct rotate_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::rotate(f, l, func);
         }
      };
      struct rotate_copy_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::rotate_copy(f, l, func);
         }
      };
      struct random_shuffle_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::random_shuffle(f, l, func);
         }
      };
      struct shuffle_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::shuffle(f, l, func);
         }
      };
      struct is_partitioned_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::is_partitioned(f, l, func);
         }
      };
      struct partition_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::partition(f, l, func);
         }
      };
      struct stable_partition_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::stable_partition(f, l, func);
         }
      };
      struct partition_copy_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::partition_copy(f, l, func);
         }
      };
      struct partition_point_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::partition_point(f, l, func);
         }
      };
      struct sort_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::sort(f, l, func);
         }
      };
      struct stable_sort_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::stable_sort(f, l, func);
         }
      };
      struct partial_sort_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::partial_sort(f, l, func);
         }
      };
      struct partial_sort_copy_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::partial_sort_copy(f, l, func);
         }
      };
      struct is_sorted_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::is_sorted(f, l, func);
         }
      };
      struct is_sorted_until_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::is_sorted_until(f, l, func);
         }
      };
      struct nth_element_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::nth_element(f, l, func);
         }
      };
      struct lower_bound_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::lower_bound(f, l, func);
         }
      };
      struct upper_bound_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::upper_bound(f, l, func);
         }
      };
      struct equal_range_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::equal_range(f, l, func);
         }
      };
      struct binary_search_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::binary_search(f, l, func);
         }
      };
      struct merge_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::merge(f, l, func);
         }
      };
      struct inplace_merge_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::inplace_merge(f, l, func);
         }
      };
      struct includes_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::includes(f, l, func);
         }
      };
      struct set_union_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::set_union(f, l, func);
         }
      };
      struct set_intersection_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::set_intersection(f, l, func);
         }
      };
      struct set_difference_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::set_difference(f, l, func);
         }
      };
      struct set_symmetric_difference_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::set_symmetric_difference(f, l, func);
         }
      };
      struct push_heap_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::push_heap(f, l, func);
         }
      };
      struct pop_heap_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::pop_heap(f, l, func);
         }
      };
      struct make_heap_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::make_heap(f, l, func);
         }
      };
      struct sort_heap_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::sort_heap(f, l, func);
         }
      };
      struct is_heap_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::is_heap(f, l, func);
         }
      };
      struct is_heap_until_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::is_heap_until(f, l, func);
         }
      };
      struct min_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::min(f, l, func);
         }
      };
      struct max_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::max(f, l, func);
         }
      };
      struct minmax_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::minmax(f, l, func);
         }
      };
      struct min_element_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::min_element(f, l, func);
         }
      };
      struct max_element_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::max_element(f, l, func);
         }
      };
      struct minmax_element_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::minmax_element(f, l, func);
         }
      };
      struct lexicographical_compare_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::lexicographical_compare(f, l, func);
         }
      };
      struct next_permutation_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::next_permutation(f, l, func);
         }
      };
      struct prev_permutation_fn
      {
         template<typename I, typename F>
         decltype(auto) operator()(I f, I l, F func) {
            return std::prev_permutation(f, l, func);
         }
      };
      struct begin_fn
      {
         template<typename C>
         decltype(auto) operator()(C&& c) {
            return std::begin(std::forward<C>(c));
         }
      };
      struct end_fn
      {
         template<typename C>
         decltype(auto) operator()(C&& c) {
            return std::end(std::forward<C>(c));
         }
      };
   }

   // To avoid ODR violations:
   template<class T>
   struct __static_const {
      static T value;
   };

   template<class T>
   T __static_const<T>::value;

   namespace
   {
      auto const& for_each = fn<3>(__static_const<__detail::for_each_fn>::value);
      auto const& begin = fn<1>(__static_const<__detail::begin_fn>::value);
      auto const& end = fn<1>(__static_const<__detail::end_fn>::value);
   }
}
#endif