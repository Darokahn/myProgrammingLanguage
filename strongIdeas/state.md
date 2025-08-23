functions can take a state as an invisible first parameter, much like object-oriented patterns.

`state` is a new type of struct which is allowed to correspond to a function. A state which corresponds to a function may be called as an alias for that function, and when called this way, it passes itself (in a pointer) as the invisible first parameter.

You can get the state required by a function by using `stateof`. `stateof function` can be used as an alias for the actual definition. This is the recommended way to use states, defining them anonymously in a function declaration and getting them for later use via `stateof`.

functions are declared as:
returntype state args name;

    void
    state {int innerValue}
    (int x, int y) func {
        ...
    }

    stateof func x = {1};
    x(0, 0);

`state`s gotten via `stateof` can be called alone, because they are tied to a specific function. But if they're defined, used as the state value in several functions, and then created as an instance later, you can use the `@` operator to attempt to call any function with that instance:

    state myState {int innerValue};

    void
    state myState
    (int x, int y) func1 {
        ...
    }

    void
    state myState
    (char* message) func2 {
        ...
    }

    void
    state {float w}
    (float x) func3 {
        ...
    }

    int () main { // note that omitted state is allowed
        state myState = {3};
        myState(); // would fail, because myState is not tied to a function
        myState@func1(0, 1); // succeeds, because we're telling it which function to use
        myState@func3(0.1f); // would fail, because the state of func3 is not the same as the one we're using.
    }

A state will always be able to see functions declared in the same namespace, even without namespace resolution.

This creates lightweight, function-first data-function unions, being somewhat agnostic between functional and OOP paradigms.
It also clearly separates which members are compiletime and which ones are stored in the state at runtim, which C++ does not do. This helps the programmer keep memory in mind when declaring states and structs.
Vtable method calls are missing from syntax, but perfectly implementable.
Inheritance is missing from syntax by design. Favor composition.
