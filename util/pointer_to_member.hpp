#ifndef _POINTER_TO_MEMBER_HPP_
#define _POINTER_TO_MEMBER_HPP_

namespace curry {

template<typename R, typename T, typename M>
struct pointer_to_member {
   
   pointer_to_member(const M& m) 
   : _m(m) {
   }

   auto& operator()(T& t) const {
      return t.*_m;
   }

   const auto& operator()(const T& t) const {
      return t.*_m;
   }

   volatile auto& operator()(volatile T& t) const {
      return t.*_m;
   }

   const volatile auto& operator()(const volatile T& t) const {
      return t.*_m;
   }

private:
   M _m;
};

}

#endif//_POINTER_TO_MEMBER_HPP_
