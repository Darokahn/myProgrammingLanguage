In C, functions don't appear as first-class types. They can in fact be used as pointed-to objects and executed dynamically, but the way they are declared seems to obfuscate this fact. I would first unify functions and function pointers. A function type variable is a first-class pointer which points to the address of executable code. Functions and function pointers are both declared in a new way:

```
    type(args) name;
```
There is a new way to define the code they point to, as well. function literals are a way to treat functions a little more equally to other data types.

Function literals will define executable memory that contains compiled machine code derived from the code inside. They will begin with a `$` symbol. They are valid inside expressions and evaluate to the pointer they resolve to at compile time. They can be cast to any function type, taking names from this cast, and the compiler will raise an error if this cannot be satisfied. If they are used in an assignment to a function variable, they cast implicitly.

```
    int(void) main = ${...};
    // or
    int(void) main = (int(void)) ${...};
```

Note that function literal syntax *only* says, at compile time:
- Create a function to put in the text segment, and
- evaluate this symbol as the pointer to that function.
It does not compile code into W^X memory at runtime.

The convention I'm leaving behind had a reason to exist; namely, it allowed compilers to make assumptions about first-class functions. Where the C compiler can make assumptions about functions explicitly declared and defined, it cannot make assumptions about function pointers that may have any value. It also aligns well with how pretty much all existing operating systems (and some architectures) assume executable memory and data memory will go in different segments, and it's intuitive with linking. With C's convention, a function name is a compiletime symbol pointing to a fixed spot in the text segment, which is literally named after that symbol, and a function pointer is a runtime value with a real slot in memory. While enough static analysis could bring my language up to speed with C's way of doing things, the developer should be able to declare this intent specifically. The `const` qualifier should essentially turn a function declaration into what it is in C: A guaranteed reference to a point in the text segment that should be named the same way in the executable header. Otherwise, the compiler will create a mangled name for the function.
