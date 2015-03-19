#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
using std::string;

#include "fn.hpp"
#include "boxed_collection.hpp"
#include "maybe.hpp"

//string greeting(string name, int answer) {
//   return "hello " + name + " and the answer is:" + std::to_string(answer);
//}
//
//int answer_func(int some_more, int and_then_some) {
//   return 42 + some_more + and_then_some;
//}
//
//int main() {
//   auto f = fn(greeting);
//   auto result = f("krishna")(_2);
//   auto result2 = result(52);
//   auto result3 = result2(fn(answer_func))(_2, _3);
//   auto result4 = result3(70);
//   auto result5 = result4(-10);
//   auto result6 = result5(fn(answer_func))(10, 10);
//   std::cout << result6 << std::endl;
//   return 0;
//}

struct A {
   A(int value) : _value(value)
   {
   }
   A(const A& a) : _value(a._value)
   {
   }
   A(A&& a) : _value(a._value)
   {
   }
   void operator=(const A& a)
   {
      _value = a._value;
   }
   void operator=(A&& a)
   {
      _value = a._value;
   }
   int _value;
};
int answer_func(int a, int b, A c, string s) {
   return a + b + c._value;
}
int simple_func(int a, int b) {
   return a + b;
}

int add_three_numbers(int a, int b, int c) {
   return a + b + c;
}

int main() {
   //A a(22);
   //auto f1 = answer_func(10, 10, a, "hello");
   //auto f2 = fn(answer_func)(10)(10)(a)("hello");
   //auto f21 = fn(answer_func)(10)(fn(answer_func))(13)(27)(a)("hi")(a)("hello");

   //std::cout << f1 << ", " << f2 << ", " << f21 << std::endl;

   //auto f3 = fn(&A::_value);
   //auto f4 = f3(a);
   //std::cout << f4;

   //auto f5 = fn(answer_func);
   //auto f6 = f5(_, 20, _, _);
   // auto f7 = f6(10, a, "hi");
   //std::cout << f7 << std::endl;

   //auto f71 = fn(simple_func);
   //auto f711 = f71(fn(simple_func));
   //auto f712 = f711(5);
   //auto f72 = f712(7);
   //auto f73 = f72(13);
   //std::cout << typeid(f73).name() << std::endl;

   //auto f8 = fn(simple_func);
   //auto f9 = f8(20);
   //auto f10 = f9(fn(simple_func));
   //auto f11 = f10(10);
   //auto f12 = f11(30);
   //std::cout << typeid(f8).name() << std::endl;
   //std::cout << typeid(f9).name() << std::endl;
   //std::cout << typeid(f10).name() << std::endl;
   //std::cout << typeid(f11).name() << std::endl;
   //std::cout << typeid(f12).name() << std::endl;

   //std::vector<int> v1 = { 19,17,21 };
   //std::vector<int> v2 = { 23,49 };
   //std::vector<int> v3 = { 7, 13 };

   //auto f13 = fn(simple_func);
   //auto f14 = f13(with_each(v1));
   //auto f15 = f14(with_each(v2));
   //std::cout << typeid(f15).name() << std::endl;

   //auto f16 = fn(add_three_numbers);
   //auto f17 = f16(with_each(v1));
   //auto f18 = f17(with_each(v2));
   //auto f19 = f18(with_each(v3));
   //std::cout << typeid(f19).name() << std::endl;

   auto print_vector = [](const auto& v) {
      for (auto&& item : v) {
         std::cout << item << ", ";
      }
      std::cout << std::endl;
   };

   std::vector<int> v1 = { 19,17,21 };
   std::vector<int> v2 = { 23,49 };
   std::vector<int> v3 = { 7, 13 };

   auto adder = fn(add_three_numbers);
   auto result = adder(with_each(v1), with_each(v2), with_each(v3));
   print_vector(result._v);

   std::vector<int> result2;
   for (auto&& i : v1) {
      for (auto&& j : v2) {
         for (auto&& k : v3) {
            result2.push_back(add_three_numbers(i, j, k));
         }
      }
   }
   print_vector(result2);

   auto value1 = maybe(10);
   auto value2 = maybe(25);
   auto value3 = maybe(30);

   auto added_maybes1 = fn(add_three_numbers)(value1, value2)(value3);
   if (added_maybes1.is_valid()) {
      std::cout << added_maybes1.get() << std::endl;
   }
   else {
      std::cout << "no result" << std::endl;
   }

   auto added_maybes2 = fn(add_three_numbers)(value1, maybe<int>())(value3);
   if (added_maybes2.is_valid()) {
      std::cout << added_maybes2.get() << std::endl;
   }
   else {
      std::cout << "no result" << std::endl;
   }
}

   //std::cout << typeid(f21).name() << std::endl;
   //for (auto&& item : result._v) {
   //   std::cout << item << ", ";
   //}
   //std::cout << std::endl;
/*
global function
member function
member data
lambda
functor

regular invocation
partial application

template<typename F, typename A1, typename A2, typename A3, typename A4>
void test_curried_application_normal(F&& f, A1&& a1, A2&& a2, A3&& a3, typename A4&& a4) {
   f(a1, a2)(a3)(a4);
   f(a1, a2, a3)(a4);
   f(a1, a2, a3, a4);
   f(a1)(a2, a3)(a4);
   f(a1, a2)(a3, a4);
   std::forward<F>(f)(std::forward<A1>(a1))(std::forward<A2>(a2))(std::forward<A3>(a3))(std::forward<A4>(a4));
}

template<typename F, typename A1, typename A2, typename A3, typename A4>
void test_curried_application_placeholders(F&& f, A1&& a1, A2&& a2, A3&& a3, typename A4&& a4) {
   f(a1, a2)(a3)(a4);
   f(a1, a2, a3)(a4);
   f(a1, a2, a3, a4);
   f(a1)(a2, a3)(a4);
   f(a1, a2)(a3, a4);
   std::forward<F>(f)(std::forward<A1>(a1))(std::forward<A2>(a2))(std::forward<A3>(a3))(std::forward<A4>(a4));
}

curried application with placeholders
curried application with placeholders switching
curried application with anonymous placeholders

curried application with composition normal
curried application with composition with placeholders
curried application with composition with placeholders switching
curried application with composition with anonymous placeholders

curried application with nested composition normal
curried application with nested composition with placeholders
curried application with nested composition with placeholders switching
curried application with nested composition with anonymous placeholders
*/
