#ifndef _SWITCH_HPP_
#define _SWITCH_HPP_
#include "std_wrappers.hpp"

namespace curry
{
   namespace __detail
   {
      struct switch_fn
      {
         template<typename Func, typename ...Cases>
         decltype(auto) operator()(Func&& func, Cases&&... cases) {
            return process(func, std::forward<Cases>(cases)...);
         }
         template<typename Case, typename ...Cases>
         decltype(auto) process(unsigned int index, Case&& c, Cases&&... cases) {
            return index ? process(index - 1, std::forward<Cases>(cases)...) : c;
         }
         template<typename Case>
         auto process(unsigned int /*index*/, Case&& c) {
            return c;
         }
      };
   }
   namespace
   {
      //template<unsigned int N>
      auto const& switch_ = fn<5>(__static_const<__detail::switch_fn>::value);
   }
}
#endif//_SWITCH_HPP_
