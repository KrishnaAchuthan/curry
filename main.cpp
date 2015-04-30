#include "fn.hpp"
#include "monads\boxed_collection.hpp"
#include "monads\maybe.hpp"
#include "operators\operators.hpp"
#include "list\immutable_list.hpp"

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
using std::string;

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
   auto lst1 = make_list<int>({ 3,4,5,6,7,8,9 });
   auto lst2 = 2 + lst1;
   auto lst3 = 1 + lst2;

   auto is_lst1_empty = empty(lst1);
   auto is_lst2_empty = empty(lst2);
   auto is_lst3_empty = empty(lst3);

   auto head2 = head(lst2);
   auto head3 = head(lst3);

   auto second_item_of_lst3 = head(tail(lst3));

   auto square = fmap(_1*_1);
   auto fmap_result = square(lst3);
   show(fmap_result);

   auto sum = foldl(_+_, 0);
   auto product = foldr(_*_, 1);

   auto sum_result = sum(lst3);
   std::cout << sum_result << std::endl;

   auto product_result = lst3 | product;
   std::cout << product_result << std::endl;

   auto f71 = fn(simple_func);
   auto f711 = f71 * simple_func;
   auto f712 = f711(5);
   auto f72 = f712(7);
   auto f73 = f72(13);
   std::cout << typeid(f73).name() << std::endl;

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

   auto added_maybes3 = value1
                      | fn(add_three_numbers)(_, maybe<int>(), value3);
   if (added_maybes3.is_valid()) {
      std::cout << added_maybes3.get() << std::endl;
   }
   else {
      std::cout << "no result" << std::endl;
   }
   
   auto added_maybes4 = fn(add_three_numbers)(_, maybe<int>(), value3) * value1;
   if (added_maybes4.is_valid()) {
      std::cout << added_maybes4.get() << std::endl;
   }
   else {
      std::cout << "no result" << std::endl;
   }

   auto print = fn([](const std::string& str) { std::cout << str << std::endl; });
   auto world = _ + std::string("world");
   "hello " | world | print;

   auto times10 = _ * 10;
   auto result21 = times10(4.2);

   auto greaterThan42 = 42 < _;
   auto result22 = 32 | greaterThan42;
}
