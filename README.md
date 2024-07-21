# ClassC
ClassC Class Langauge Extension Compiler for C, 

Its c with a couple of extensions to allow Class containers

By default everything is public.

Variables may also always be public as they will just be defined in a struct

** new keywords **
class
interface
signature

** new syntax **
@ annotations

all additional languge features will be included with annotations
@public
@private
@static
@final
@const
@throws
@getter
@setter

? auto type assign
i.e
?myVar = 10; would be int myVar = 10;


=<varname> constructors

`MyClass(=myVar);`

creates a constructor which accepts a paramater of same type as myVar and set myVar to the value passed.

** basic layout **

```
class <class_name> ([extends, implements..]) {
    [variables definitions]
    [constructor definitions]
    [function definitions]
}

[anything that is here (outside a class definition) is treated as c]
```
e.g
```
class MyClass {
    int myVar;`

    MyClass(=myVar);

    void add(int value) {
        myVar += value;
    }
}
```

## Compiler ##
Compiles ClassC to c

## Road Map ##

** Aplha 0.1 **
Base Lexer, Paser, Compiler
* No Annotations
* No extends

** Alpha 0.2 **
Extends

** Alpha 0.3 **
Annotations

** Alpha 0.4 **
Ref counting and auto cleanup

** Alpha 0.5 **
Parameter default values
`void myFunction(int x =0, int y = 0);`

** Aplha 0.x **
Basic Standard library written in ClassC

At this stage I am thinking something which uses MUSL, but directly cals the syscall functions for kernel functions.

** Beta 0.1 **
re-write compiler from c to ClassC

## compiler output ##
c file
```
functions...
<return> <classname>_<functionname>(<parameters>)

constructors...
init...
```
h file
```
struct <classname>_Data {
    variables...
}

struct <classname>_Functions {
    function defs...
}
```

signature file
This is the ClassC equivalent to a header file
```
signature MyClass {
    int myVar;
    void myFunction();
}
```
compile stript

GCC, clang, MSVC etc