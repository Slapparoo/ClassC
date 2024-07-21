# ClassC
ClassC Class language Extension Compiler for C, 

Its c with a couple of extensions to allow Class containers.

So with very little manipulation the compiler will output c code, which can then be compiled using your favourite c language compiler (at this stage I am thinking C17)

By default everything is public.

Everything compiled will be available in C and Visa Versa - inside c some patterns will need to be adhered to.

**new keywords**

* `class` definition
* `interface` - interface definition
* `signature` - synonymous with a header file, and used to reference precompiled classes

**new syntax**

@ annotations

all additional language features will be included with annotations
@public
@private
@static
@final
@const
@throws
@getter
@setter

The idea being learning and using will be simple and remain simple, but complexity is available if required.

? auto type inference
i.e
`?myVar = 10;` would become `int myVar = 10;`

`=<varname>` constructors parameters, with type inference

`MyClass(=myVar);`

creates a constructor which accepts a parameter of same type as myVar and set myVar to the value passed.

**basic layout**

```c
class <class_name> ([extends, implements..]) {
    [variables definitions]
    [constructor definitions]
    [function definitions]
}

[anything that is here (outside a class definition) is treated as c]
```
e.g
```c
class MyClass {
    // Variables - standard c variable definitions 
    int myVar;

    // Constructors 
    MyClass(=myVar);

    // functions standard c function definitions
    void add(int value) {
        myVar += value;
    }
}
```
Or the following to extend Object, and implement OtherFunction,
at compile time it can easily determine if something is a class or interface

The standard familiar parameter passing pattern is used.
```c
class MyClass (Object, OtherFunction) {
    int myVar;

    MyClass(=myVar);

    void add(int value) {
        myVar += value;
    }
}
```

## Compiler ##

Compiles ClassC to c

## Road Map ##

**Alpha 0.1**

Base Lexer, Parser, Compiler
* No Annotations
* No extends

**Alpha 0.2**

Extends, implements

**Alpha 0.3**

Annotations - compile time
- not sure if there will be runtime I haven't explored that far into it, initially there will be no runtime introspection.

**Alpha 0.4**

Ref counting and auto clean-up

**Alpha 0.5**

Parameter default values
`void myFunction(int x = 0, int y = 0);`

which allows a function to be called as:
```C
int x = 10, y = 20;
myFunction(x, y);
myFunction(x);
myFunction();
```

**Alpha 0.x**

Basic Standard library written in ClassC

At this stage I am thinking something which uses MUSL, but directly calls the `syscall` functions for kernel functions.

**Beta 0.1**

re-write compiler from c to ClassC

## compiler output ##

c file
```c
functions...
<return> <classname>_<functionname>(<parameters>)

constructors...
init...
```
h file
```c
struct <classname>_Data {
    variables...
}

struct <classname>_Functions {
    function defs...
}
```

signature file
This is the ClassC equivalent to a header file
```c
signature MyClass {
    int myVar;
    void myFunction();
}
```
compile script

GCC, clang, MSVC etc