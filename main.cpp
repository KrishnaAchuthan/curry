#include <iostream>
#include <string>
#include <typeinfo>
using std::string;

#include "fn.hpp"

//string greeting(string name, int answer) {
//	return "hello " + name + " and the answer is:" + std::to_string(answer);
//}
//
//int answer_func(int some_more, int and_then_some) {
//	return 42 + some_more + and_then_some;
//}
//
//int main() {
//	auto f = fn(greeting);
//	auto result = f("krishna")(_2);
//	auto result2 = result(52);
//	auto result3 = result2(fn(answer_func))(_2, _3);
//	auto result4 = result3(70);
//	auto result5 = result4(-10);
//	auto result6 = result5(fn(answer_func))(10, 10);
//	std::cout << result6 << std::endl;
//	return 0;
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

int main() {
	A a(22);
	auto f1 = answer_func(10, 10, a, "hello");
	auto f2 = fn(answer_func)(10)(10)(a)("hello");
	auto f21 = fn(answer_func)(10)(fn(answer_func))(13)(27)(a)("hi")(a)("hello");

	std::cout << f1 << ", " << f2 << ", " << f21 << std::endl;

	auto f3 = fn(&A::_value);
	auto f4 = f3(a);
	std::cout << f4;

	auto f5 = fn(answer_func);
	auto f6 = f5(_, 20, _, _);
    auto f7 = f6(10, a, "hi");
	std::cout << f7 << std::endl;

	auto f71 = fn(simple_func);
	auto f711 = f71(fn(simple_func));
	auto f712 = f711(5);
	auto f72 = f712(7);
	auto f73 = f72(13);
	std::cout << typeid(f73).name() << std::endl;

	auto f8 = fn(simple_func);
	auto f9 = f8(20);
	auto f10 = f9(fn(simple_func));
	auto f11 = f10(10);
	auto f12 = f11(30);
	std::cout << typeid(f8).name() << std::endl;
	std::cout << typeid(f9).name() << std::endl;
	std::cout << typeid(f10).name() << std::endl;
	std::cout << typeid(f11).name() << std::endl;
	std::cout << typeid(f12).name() << std::endl;
}
