#include <iostream>
#include <string>
using std::string;

#include "fn.hpp"

#include <string>
#include <iostream>
using std::string;

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
	auto result3 = result2(fn(answer_func));
	auto result4 = result3(10);
	auto result5 = result4(10);
	//std::cout << result4 << std::endl;
	std::cout << result5 << std::endl;
	return 0;
}