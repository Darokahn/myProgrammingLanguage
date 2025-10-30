here's a radical idea to let systems-level languages opt in to the universal reflection of interpreted languages. A `reflected` keyword would allow types to be declared via runtime strings, and the compiler would essentially take its hands off the wheel of defining offsets at comptime and link in a small interpreter for this use case. The general vibe of this feature would be, "using this is wrong most of the time, but many programs do it manually, so let's define a formality for it and take steps to stop it from being abused."

This feature could be useful so that data formats can be defined externally and the interpreting script doesn't need to know about them. It would not affect the strict typing of the rest of the language, and the language would simply bundle in a type-safe interpreter. The programmer should be strongly encouraged to keep reflected types isolated and few, kind of like how functional programming languages treat monads. They would be essentially a useful tool to do the dirty work without being the device for the computation the rest of the program needs. Programmers could and would abuse it, but its purpose would be to exist in the corner or init phase of a codebase, not drive the whole thing.

```c
    reflected <runtime string (maybe read from a file?)> myStruct;
    int fileSize = myStruct.size // only fileSize is exported to the rest of the program, the reflected struct is abandoned after this function returns the size.
    return fileSize;
```

The `reflected` type would be allocated via dynamic stack allocation, which is generally frowned upon, but we're already way over that hill.

I would generally want to trust the programmer, but it may be worthwhile to put in some guards, or at least warnings, to let them know they're using reflected types too liberally and removing the ability of their program to run smoothly with comptime optimizations. Essentially, "You're pretty much using Python at this point."

Defining reflected types at the module level is probably abuse. You should not have reflected types that last the whole program.

Defining reflected types inside structs, at the module level, is especially abuse.

Defining reflected types that appear to be assumed scalar is probably abuse.

There would also need to be a good mechanism for detecting when a reflected type can't be used as-specified, and returning properly. I will write about that later. Possibly a `try/except` deal or a required ABI for functions that use reflected types (their return type is fixed to the REFLECTIONSTATUS struct containing an error code, a formatted string, and a `va_args` object, so they can't be pure and must take pointers to modify. This is probably what I'll go with, as it prioritizes just printing what went wrong and then exiting).

There could also be a simple `reflect` keyword that adds a type to a runtime registry of relevant information, including a unique type ID, a formatted string of its layout, and its name. This information would be inherently available for the lifetime of any `reflected` type, but static types could opt in to that information being logged about them as well. This data could be fetched using `typeof`, `formatof`, and `nameof` operators.

While most of that could be resolved at comptime, those operators would also correspond to runtime functions, which could be called with an ID derived from a type. This would allow comptime-managed `void*` polymorphism, where the ID of the data type is stored with the pointer. Then, the user doesn't need their own enum, the compiler manages it.

The `reflect` qualifier would encourage users to still use static types where they might've otherwise grabbed for a fully reflected type.
