###Support for Curry and Function Composition in C++

####What is this?
Functional programming languages such as Haskell has features such as currying, function composition etc. This library is an attempt to bring such features into C++ using modern techniques available in C++14.

####Usage
This is a header only library. Just include the file fn.hpp in your source code. The main function 'fn' and the related components are available inside the namespace 'curry'

####Tools needed
This project uses C++ 14 language features. It is successfully compiled under MSVC 2015, Clang 3.6.0 and GCC 4.92 (using the std=c++1y switch) 

####Notes
#####1. Enabling currying - Global, Member, Functor and Lambda Functions as well as Member Variables.

```cpp
    auto f1 = fn(foo);            //Global function.
    auto f2 = fn(&A::bar);        //Member function of a class.
    auto f3 = fn(B());            //Functor
    
    auto lambda = [](auto arg1) {
       return arg1*10.0;
    };
    auto f4 = fn(lambda);         //Lambda. Anonymous lambda works too.
    
    //Binding a member variable. Creates a one argument function.
    auto f5 = fn(&std::pair::second); 
```

#####2. Partial application of Functions

```cpp
    //foo is a function that takes four arguments. Two are partially applied here.
    auto partially_applied = fn(foo, arg1, arg2);
    
    //The remaining two arguments are supplied here. Calls foo(arg1, arg2, arg3, arg4);
    partially_applied(arg3, arg4);
```

#####3. Currying of Functions

```cpp
   //foo is a function that takes four arguments.
   auto curried_function = fn(foo); 
   
   auto one_arg_applied              = curried_function(arg1);
   auto some_more_applied            = one_arg_applied(arg2, arg3);
   auto the_rest_applied_and_invoked = some_more_applied(arg4);
```

#####4. Placeholders

```cpp
   auto f1 = fn(foo);                      //foo takes four arguments.
   auto f2 = f1(_4, _1, _3, _2);           //Reorder arguments.
   auto f3 = f2(arg1, arg2, arg3, arg4);   //Calls foo(arg4, arg1, arg3, arg2)
```

#####5. Anonymous Placeholders

```cpp
   auto f1 = fn(foo);                      //foo takes four arguments.
   auto f2 = f1(_, arg1, arg2, _);         //Some arguments are applied. It waits for more.
   
   //Remaining arguments provided. Invokes foo(arg3, arg1, arg2, arg4)
   auto f3 = f2(arg3, arg4);
```

#####6. Function Composition ( Using composition operator '*' )

```cpp
   //foo takes four arguments
   auto f1 = fn(foo);
   
   //bar is a function that takes two arguments. It is composed in.
   auto f2 = f1(arg1, _, arg2, arg3) * bar;
   
   //Calls foo(arg1, bar(arg4, arg5), arg2, arg3)
   auto f3 = f2(arg4, arg5);
```

#####7. With Each ( Binding containers to values just like Monads and Applicative Functors )

```cpp
   int add_three_numbers(int a, int b, int c) {
      return a + b + c;
   }

   std::vector<int> v1 = { 19,17,21 };
   std::vector<int> v2 = { 23,49 };
   std::vector<int> v3 = { 7, 13 };

   auto adder  = fn(add_three_numbers);
   auto result = adder(with_each(v1), with_each(v2), with_each(v3));

   //result will be a vector with 12 values {49, 55, 75, 81, 47, 53, 73, 79, 51, 57, 77, 83}
```


#####8. Maybe

```cpp
   int add_three_numbers(int a, int b, int c) {
      return a + b + c;
   }

   auto value1 = maybe(10);
   auto value2 = maybe(20);
   auto value3 = maybe(30);

   auto adder  = fn(add_three_numbers);
   auto result = adder(value1, value2, value3);

   //result will be maybe_t<int>(60)

   auto value4 = maybe(10);
   auto value5 = maybe_t<int>();
   auto value6 = maybe(30);

   auto adder  = fn(add_three_numbers);
   auto result = adder(value4, value5, value6);

   //result will be maybe_t<int>() which is empty since value5 is empty
   
```

#####9. Pipe Operator '|'

```cpp
   auto print = fn([](const std::string& str) { std::cout << str << std::endl; });
   auto world = _ + std::string("world");
   "hello " | world | print; //prints "hello world" to the console
```

#####10. On the fly function creation with placeholders

```cpp
   auto times10 = _ * 10;
   auto result1 = times10(4.2); //result1 will be 42.0
   
   auto greaterThan42 = 42 < _;
   auto result2 = 32 | greaterThan42; //result2 will be false
```

#####11. Immutable list (Code inspired by Bartosz Milewski's SimpleList)

```cpp
   auto lst = make_list<int>({1,2,3,4,5,6,7,8,9});
   
   auto lst2 = tail(lst); //lst2 will be [2,3,4,5,6,7,8,9]
   auto val = head(lst2); //val will be 2
   auto lst3 = 1 + lst2; //lst3 will be [1,2,3,4,5,6,7,8,9]
   
   auto is_lst_empty = empty(lst); //result will be false

   auto second_item_of_lst3 = head(tail(lst3)); //result will be 2
```

#####12. Higher Order Functions

```cpp
   auto lst = make_list<int>({1,2,3,4,5,6,7,8,9});

   auto square = fmap(_1*_1); //on the fly function creation with curried fmap application
   auto fmap_result = square(lst); //result will be [1,2,9,16,25,36,49,64,81]

   auto sum = foldl(_+_, 0);     //curried foldl application
   auto product = foldr(_*_, 1); //curried foldr application

   auto sum_result = sum(lst);  //result will be 45
   std::cout << sum_result << std::endl;

   auto product_result = lst | product; //result will be 362880
   std::cout << product_result << std::endl;
```
