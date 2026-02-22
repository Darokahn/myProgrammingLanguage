# `state` is a property of a function parameter. Function parameters that are the function's `state` have special properties.

A function's parameter can become its state if it is the first parameter, AND (

The parameter is called `state`, OR

The parameter is not named at all.

)

Inside a function which has a state, if that state has members (i.e. it is a struct), those members can be referred to via `.member` rather than `state.member`.

This is a separate property of the language, but `state->member` is unnecessary if `state` is a reference type. Its members can be accessed via `.` as if it was a value.

This document assumes a few other properties of the language that may appear surprising. They can often be reasoned about, but the full details about them can be found among the other `.md` files in this directory.

The type of a function's state can be taken via `stateof`. It is used as if a a text-replacement preprocessor had pasted the parameter's type in place of the `stateof func` expression. 

A natural pattern will be to use an anonymous type as the state variable of a function and get it back later via `stateof`.

While acting like a text replacement for the type of a function's `state`, `typeof` also gives any variable declared via this replacement the property of being an alias to the function itself.

It is fully usable as a variable of the specified type, but it is also callable. It calls the function it was derived from, passing itself as the state parameter.

```
    int(ref struct {int x; int y;}, int someParam) myFunc = ${
        return .x + .y + someParam;
    }

    int() main = ${
        stateof myFunc x = {1, 3};
        printf("%d\n", x(4));
    }
```

As you can see, `x` is a lot like a closure that encloses specifically the parameters you tell it to.

If a variable is not tied to a specific function, but there are several functions whose states are the same type, those functions can of course be called with that variable as their first parameter. Alternatively, the `@` operator is a binary operator which calls its rhs with the lhs as its first parameter. Taking advantage of this can create object-oriented patterns which are very similar to in other languages.

```
    struct myStruct {
        int x;
        int y;
    };

    void(myStruct) print = ${
        printf("x: %d, y: %d\n", .x, .h);
    }

    int() main = ${
        myStruct s;
        s@print();
    }
```

The `@` operator can see inside the namespace of the lhs operand. The name of the rhs operand can match a declaration from that namespace without resolving it.

Note that `_` is the namespace resolution character.

```
    namespace myStruct {
        struct t {
            int x;
            int y;
        };
        void(t) print = ${
            printf("x: %d, y: %d\n", .x, .h);
        }

    }

    int() main = ${
        myStruct_t s;
        s@print();
    }
```

There is no virtual function call table. Instead, developers should create a member of a struct which is the pointer to a function, and dispatch `struct@struct.vfunc()` calls.
