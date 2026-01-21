# functions can take a state as an invisible first parameter, much like object-oriented patterns.

`state` is a new type of struct which is allowed to correspond to a function. A state which corresponds to a function may be called as an alias for that function, and when called this way, it passes itself (in a pointer) as the invisible first parameter.

Functions with `usestate` use `this`, but with some differences to how it is commonly used.

Due to the semantics of references outlined in `references.md` (which I intend to compose with this idea), immutable singular pointers are dereferenced automatically. You can use `struct.member` rather than requiring  `struct->member`. Therefore, you use `this.member` instead of `this->member`. A third option is that, in state-having functions, plain `.member` is implicitly `this.member`.

Remember that for most purposes, a `state` is equivalent to a struct. It has additional features, but you can reason about things like lifetime, layout, etc. as if it was a struct.

You can get the state required by a function by using `stateof`. `stateof function` can be used as an alias for the actual definition. This is the functional programming oriented way to use states, defining them anonymously in a function declaration and getting them for later use via `stateof`.

functions are declared as:

`returntype usestate args name;`

```c
    void
    usestate {int innerValue;}
    (int x, int y) 
    myFunc {
        printf("%d, %d, %d\n", x, y, .innerValue);
    }

    stateof myFunc x = {1};
    x(0, 0);
```

This makes function declarations that use state somewhat heavy. This is okay, and I don't think there's a better way to explicitly define the information a function state needs to use.

This language takes after C++ when declaring struct data types, but not state. struct data types can be specified by either `struct name` or just `name` without a typedef, exactly as C++. However, `state` variables must always be qualified by `state`. In function declarations, `usestate state T` is redundant, and `usestate` is therefore a specialized version of `state` that also means "this function *uses* this state".

The shape of a declaration alone is enough for the compiler to know how to treat it:

- First, it consumes the return type. At this point, it doesn't know whether this declaration is a function or not.
- Second, it knows that the next character is either open parentheses for the params list (this function has no state) or a name. If it finds a name, it checks whether the name is `usestate`. If it is, this is a state function. If not, this is a regular variable. Unlike C, type names become reserved after definition and cannot be used for instances.
- Third, it verifies that the rest of the declaration can satisfy the assumption it made by peeking at the next token type. If the name does not specify a state type, this is not a valid function declaration. If the parentheses do not enclose valid parameter definitions, it is likewise not valid.

It is recommended to declare the components of state-having functions on separate lines for clarity.

`state`s gotten via `stateof` can be called alone, because they are tied to a specific function. But if they're defined, used as the state value in several functions, and then created as an instance later, you can use the `@` operator to attempt to call any function with that instance:

    state myState {
        int innerValue;
    };

    void
    usestate myState
    (int x, int y) 
    func1 {
        .innerValue += x + y;
    }

    void
    usestate myState
    (char* message) func2 {
        ...
    }

    void
    usestate {float w;}
    (float x) func3 {
        ...
    }

    int () main { // note that omitted state is allowed
        state myState m1 = {3};
        stateof func1 m2;
        m1(); // would fail, because m1 is not tied to a function
        m2(); // succeeds, becuase m2 is tied to a function
        m1@func1(0, 1); // succeeds, because we're telling it which function to use
        param1@func(rest of params)
        m1@func3(0.1f); // would fail, because the state of func3 is not the same as the one we're using.
    }

A state will always be able to see functions declared in the same namespace, even without namespace resolution.

This creates lightweight, function-first data-function unions, being somewhat agnostic between functional and OOP paradigms.
It also clearly separates which members are compiletime and which ones are stored in the state at runtime, which C++ does not do. This helps the programmer keep memory in mind when declaring states and structs.
Vtable method calls are missing from syntax, but perfectly implementable.
Inheritance is missing from syntax by design. Favor composition. See notes on compositions in composition.md.

`state` structs can specify a function they call when initialized. I am chagrin to borrow from C++, but the way it works there is pretty good. When the function your `state` struct refers to takes several arguments, you pass them at initialization with parentheses after the variable name.

`call func` creates weak forward declaration with the signature `void usestate T ()`. This is re-declarable, as long as the specified parts match. You can also just define it if that declaration is suitable.

```
    namespace apple {
        state instance {
            int inner;
        } call init;

        init = ${
            .inner = 0;
        };
    }

    main = ${
        // this calls init
        apple_instance a;
    };
```
