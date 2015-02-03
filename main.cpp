#include "fn.hpp"
#include <iostream>
#include <string>
using std::string;

string greeting(string name, int answer) {
   char buffer[64];
   return "hello " + name + " and the answer is: " + itoa(answer, buffer, 10);
}

int answer_func() {
   return 42;
}

void main() {
   auto f = fn(greeting);
   auto result = f("krishna")(_2);
   auto result2 = result(52);
   auto result3 = result2(fn(answer_func));
   std::cout << result3 << std::endl;
}
