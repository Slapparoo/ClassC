# ClassC
ClassC Class Langauge Extension Compiler for C, 

Its c with a couple of extensions to allow Class containers.

So with ver little maunipulation the compiler will output c code, which can then be compiled using your favorite c langauge compiler (at this stage I am thinking C17)

By default everything is public. and as all the features are added can get anannotation to make them `@private`

Everything compiled will be available in C and Visa Versa - inside c some patterns will need to be adheared to.

Variables may endup always be public as they will just be defined in a struct, so when used in ClassC could be 'gaurded' but always available in c.

**new keywords**

class - class definition
interface - interface definition
signature - synonimous with a header file, and used to reference precompiled classes

**new syntax**

@ annotations

all additional langauge features will be included with annotations
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

`=<varname>` constructors parameters, with type inferance

`MyClass(=myVar);`

creates a constructor which accepts a paramater of same type as myVar and set myVar to the value passed.

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
    int myVar;

    MyClass(=myVar);

    void add(int value) {
        myVar += value;
    }
}
```
or the following to extend Object, and implement OtherFunction,
at compile time it can easily determine if something is a class or interface
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

**Aplha 0.1**

Base Lexer, Paser, Compiler
* No Annotations
* No extends

**Alpha 0.2**

Extends, implements

**Alpha 0.3**

Annotations

**Alpha 0.4**

Ref counting and auto cleanup

**Alpha 0.5**

Parameter default values
`void myFunction(int x =0, int y = 0);`

**Aplha 0.x**

Basic Standard library written in ClassC

At this stage I am thinking something which uses MUSL, but directly cals the syscall functions for kernel functions.

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
compile stript

GCC, clang, MSVC etc