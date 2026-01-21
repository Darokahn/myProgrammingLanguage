Immutable types can be initialized, but they can't be assigned to after initialization. Sometimes, you need to declare something, then initialize it later, but you still generally want it to be immutable. I propose the `init` keyword, which is allowed to assign to a mutable type as long as certain conditions are verifiable:

1. The scope `init` is used in can see the declaration of the variable (init is valid on struct fields) 

2. The variable was declared without an initializer 

3. The init does not appear in a code path where a prior init could even possibly have initialized it already. The compiler would statically verify these rules and then allow the assignment.

```c
    ref[int] x; // `ref[int]` is conceptually int *, with compiler-enforced rule that you cannot reassign it, do arithmetic on it, or index into it.
                // C-style pointer declarations are not valid; one of the sugared pointer types `arr`, `slc`, `ptr`, `ref`, `seq` must be used, or
                // A fully qualified `addr` with all rules specified:
                // `<mut | immut> <singular | plural[n]> <indirect | embedded> addr[T]`
    if (selection == 0) {
        init x = &a;
    }
    else {
        init x = &b;
    }
```

A function may use `init` on the fields of a struct or struct pointer parameter. However, the same rules apply. The function *can* see the declaration of that struct/struct ref, because the caller promises not to have initialized it. If a caller initializes any struct members, then passes that struct or a pointer to it to a function that initializes members, the compiler forbids it.

```c
    struct x {
        ref[int] a;
        ref[int] b;
    };

    // My function initialization syntax
    void(ref[struct x] x, int a, int b) init = ${
        init x.a = &a;
        init x.b = &b;
    }

    struct x myStruct;

    int a = 0;
    int b = 1;
    init(&myStruct, &a, &b); // allowed, because myStruct has no initialized members.
```
