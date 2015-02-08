#include <iostream>
#include <string>
#include <typeinfo>
using std::string;

#include "fn.hpp"

string greeting(string name, int answer) {
	char buffer[64];
	return "hello " + name + " and the answer is:" + itoa(answer, buffer, 10);
}

int answer_func(int some_more, int and_then_some) {
	return 42 + some_more + and_then_some;
}

int main() {
	auto f = fn(greeting);
	auto result = f("krishna")(_2);
	auto result2 = result(52);
	auto result3 = result2(fn(answer_func))(_2, _3);
	auto result4 = result3(70);
	auto result5 = result4(10);
	auto result6 = result5(-10);
	std::cout << result6 << std::endl;
	return 0;
}