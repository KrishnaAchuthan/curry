#ifndef _APPLY_ARGUMENTS_HPP_
#define _APPLY_ARGUMENTS_HPP_

#include "..\util\get_conditionally.hpp"
#include "..\util\arg_start_indices.hpp"
#include "..\util\int_list.hpp"
#include <tuple>

namespace curry {

template<int ArgsCount, int ...I, typename T, typename A>
CONSTEXPR decltype(auto) apply_arguments(int_list<I...>, T&& t, A&& a) {
   return std::make_tuple(get_conditionally<ArgsCount, 
                                           std::tuple_element_t<I, typename arg_start_indices<T>::type>
                                           >
                            (arg_category<decltype(std::get<I>(t))>(), 
                        std::get<I>(std::forward<T>(t)), 
                        std::forward<A>(a)
                       )...
                    );
}

}

#endif//_APPLY_ARGUMENTS_HPP_