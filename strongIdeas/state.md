# functions can take a state as an invisible first parameter, much like object-oriented patterns.

functions can be tagged "stateful", which grants special properties to their first argument.

1. The parameter must be called `state`, and you do not need to specify its name (type only is okay). It is best to omit the name, as the only name that would be accepted is `state`. Its type can be given as an argument to the `stateful` keyword. This option is always available, but is mainly for use with another feature that will be revealed further in the document.
2. The parameter must be a singular immutable indirect reference type.
3. Due to an orthogonal property of the language, the state can be used like a value despite being a reference. See `references.md` for more.
4. `state.member` is unnecessary and can be replaced with `.member`

This document assumes a few other properties of the language. They are not incredibly relevant but may appear surprising. They can largely be reasoned about, and their descriptions can be found in the various `.md` files in this directory.

You can get the state required by a function by using `stateof`. `stateof function` can be used as an alias for the actual definition. This is the functional programming oriented way to use states, defining them anonymously in a function declaration and getting them for later use via `stateof`.

```
    stateful int(ref[struct {int x; int y;}], int someParam) myFunc = ${
        return .x + .y + someParam;
    }
```

states gotten via `stateof` are a way to retrieve anonymous structs and other types declared inside function signatures. `stateof func` becomes whatever type the function uses as state, almost as if via text replacement. However, the compiler remembers which function they are derived from.

states gotten via `stateof` can act like closures for the function they are derived from. 

Because it is tied to a specific function, the state can be called as an alias for that function. Doing so naturally passes the state as the first parameter. 

If a type is defined, used as the state value in several functions, and then created as an instance later, you can use the `@` operator to attempt to call any function with that instance:

    struct myState {
        int innerValue;
    };

    stateful void(ref[myState], int x, int y) func1 {
        .innerValue += x + y;
    }

    stateful void(ref[myState], char* message) func2 {
        ...
    }

    stateful void (ref[struct {float w;}], float x) func3 {
        ...
    }

    int () main { // note that omitted state is allowed
        ref[myState] m1 = {3};
        stateof func1 m2 = {1};
        m1(); // would fail, because m1 is not tied to a function
        m2(); // succeeds, because m2 is tied to a function
        m1@func1(0, 1); // succeeds, because we're telling it which function to use
        param1@func(rest of params)
        m1@func3(0.1f); // would fail, because the state of func3 is not the same as the one we're using.
    }

This is simply a method-call-like ergonomic feature. You can always pass them as the first parameter as well. In fact, `@` is just a sugar for doing so, with one important distinction.

`@` is willing to coerce any pointer type to any other pointer type implicitly. Due to a technicality of the reference system this language uses, even values are technically pointers. This allows you not to bother with taking the address of a struct every time you use `@method` (recall that the type used as a state must be a reference).

A type will always be able to see functions declared in the same namespace, even without namespace resolution.

This creates lightweight, function-first data-function unions, being somewhat agnostic between functional and OOP paradigms.
It also clearly separates which members are compiletime and which ones are stored in the state at runtime, which C++ does not do. This helps the programmer keep memory in mind when declaring states and structs.
Vtable method calls are missing from syntax, but perfectly implementable.
Inheritance is missing from syntax by design. Favor composition. See notes on compositions in composition.md.

`stateful(type)` can be used to create a block of function declarations that all use the same state.

`stateful(ref[struct {int a; int b;}]) {
    void(int a, int b) func1 = ${...};
    int(int x, float y) func2 = ${...};
}
