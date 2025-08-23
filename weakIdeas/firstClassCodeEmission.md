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

