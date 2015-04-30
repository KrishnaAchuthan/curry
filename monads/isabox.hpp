#ifndef _IS_A_BOX_HPP_
#define _IS_A_BOX_HPP_

#include <type_traits>

template<typename T>
struct isabox : std::false_type {
};

#endif//_IS_A_BOX_HPP_