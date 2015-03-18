###Support for Curry and Function Composition in C++

#####1. Global, Member, Functor and Lambda Functions as well as Member Variables.

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

#####2. Partial application of functions

```cpp
    //foo is a function that takes four arguments. Two are partially applied here.
    auto partially_applied = fn(foo, arg1, arg2);
    
    //The remaining two arguments are supplied here. Calls foo(arg1, arg2, arg3, arg4);
    partially_applied(arg3, arg4);
```

#####3. Currying of functions

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
   
   //Remaining arguments provided. Invokes foo(arg3, arg1, arg3, arg4)
   auto f3 = f2(arg3, arg4);
```

#####6. Function composition

```cpp
   //foo takes four arguments
   auto f1 = fn(foo);
   
   //bar is a function that takes two arguments. It is composed in.
   //[working on a better syntax for this, using a composition operator...]
   auto f2 = f1(arg1, fn(bar), arg2, arg3);
   
   //Calls foo(arg1, bar(arg4, arg5), arg2, arg3)
   auto f3 = f2(arg4, arg5);
```

#####7. Boxes (Monad???, Applicative Functors???)

```cpp
   int add_three_numbers(int a, int b, int c) {
      return a + b + c;
   }

   std::vector<int> v1 = { 19,17,21 };
   std::vector<int> v2 = { 23,49 };
   std::vector<int> v3 = { 7, 13 };

   auto adder  = fn(add_three_numbers);
   auto result = adder(with_each(v1), with_each(v2), with_each(v3));

   //result will be a vector with 12 values { 49, 55, 75, 81, 47, 53, 73, 79, 51, 57, 77, 83 }
```

####Future Features Planned

#####1. Composition operator *

#####2. Pipeline operator |

#####3. 
