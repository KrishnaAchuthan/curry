#ifndef _PLACEHOLDER_HPP_
#define _PLACEHOLDER_HPP_

template<int>
struct ph {};

ph<0> _;
ph<1> _1;
ph<2> _2;
ph<3> _3;
ph<4> _4;
ph<5> _5;
ph<6> _6;
ph<7> _7;
ph<8> _8;
ph<9> _9;
ph<10> _10;

template<typename T>
struct is_placeholder {
   static const bool value = false;
};

template<int I>
struct is_placeholder<ph<I>> {
   static const bool value = true;
};

#endif//_PLACEHOLDER_HPP_
