#ifndef _ARG_POSITIONS_HPP_
#define _ARG_POSITIONS_HPP_

#include <tuple>

namespace curry {

template<typename Result, int Count>
struct arg_positions_impl;

template<typename ...Result, int Count>
struct arg_positions_impl<std::tuple<Result...>, Count> {
   using type = typename arg_positions_impl<std::tuple<ph<Count>, Result...>, Count - 1>::type;
};

template<typename ...Result>
struct arg_positions_impl<std::tuple<Result...>, 0> {
   using type = std::tuple<Result...>;
};

template<int Count>
struct arg_positions {
   using type = typename arg_positions_impl<std::tuple<>, Count>::type;
};

}

#endif//_ARG_POSITIONS_HPP_