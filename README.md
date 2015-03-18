###Support for Curry and Function Composition in C++

#####1. Global, Member, Functor and Lambda Functions. as well as Member Variables

```cpp
    auto f1 = fn(foo);                                 //foo is a global function
    auto f2 = fn(&A::bar);                             //bar is a member function of class or struct A
    auto f3 = fn(B());                                 //B is a class or struct with an overloaded () operator
    auto f4 = fn([](auto arg1) { return arg1*10.0; }); //anonymous lambda
    auto f5 = fn(&std::pair::second)                   //creates a one argument function 
```

#####2. Partial application of functions

```cpp
    //foo is a function that takes four arguments. Two are partially applied here
    auto partially_applied = fn(foo, arg1, arg2);
    
    //The remaining two arguments are supplied here. Calls foo(arg1, arg2, arg3, arg4);
    partially_applied(arg3, arg4);
```

#####3. Currying of functions

```cpp
   //foo is a function that takes four arguments
   auto curried_function = fn(foo); 
   
   auto one_arg_applied = curried_function(arg1);
   auto some_more_applied = one_arg_applied(arg2, arg3);
   auto the_rest_applied_and_invoked = some_more_applied(arg4);
```

#####4. Placeholders

```cpp
   auto f1 = fn(foo);                      //foo takes four arguments
   auto f2 = f1(_4, _1, _3, _2);           //Arguments passed in are reordered while calling foo
   auto f3 = f2(arg1, arg2, arg3, arg4);   //Calls foo(arg4, arg1, arg3, arg2)
```

#####5. Anonymous Placeholders

```cpp
   auto f1 = fn(foo);                //foo takes four arguments
   auto f2 = f1(_, arg1, arg2, _);   //Some arguments are applied and waits for more
   auto f3 = f2(arg3, arg4);         //Calls foo(arg3, arg1, arg3, arg4)
```

#####6. Function composition

```cpp
   //foo takes four arguments
   auto f1 = fn(foo);
   
   //bar is a function that takes two arguments. It is composed in.
   //[working on a better syntax for this, using a composition operator...]
   auto f2 = f1(arg1, fn(bar), arg2, arg3);
   
   // calls foo(arg1, bar(arg4, arg5), arg2, arg3)
   auto f3 = f2(arg4, arg5);
```

