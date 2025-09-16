# `using` and implicit conversions

A good language is strongly typed, and my language intends to be. However, in my language, by defining a conversion table with `using`, you can inform the compiler which implicit conversions to use, and what expression to use for conversion. `using` blocks should encapsulate specific code that wants convenient casting, not a whole project. I haven't come up with the syntax yet, but a `using` table can be predefined once and deployed where needed. Probably quite similar to defining and using structs.

    struct myData {
        int x;
        char* message;
    };
    char buffer[1024];
    int index = 0;

    char* (int a, char* buffer, int* index) intToString {
        // implement a function that writes `a` as a string into buffer, starting at index, and returns buffer + initial index.
        // Also sets index to the new next spot
    }

    using (
        int a -> char*: intToString(a, buffer, &index); // `a` is an arbitrary placeholder and is not declared/visible outside this line
        struct myData a -> int: a.x;
        struct myData a -> char*: a.message;
    ) {
        struct myData d = {.x = 10, .message="hello, world!"};
        int x = 10;
        puts(x);
        puts(d);
    }
    puts(d) // This won't work, because it's outside the `using` block.

An alternative to using a conversion table would be the "type resolution" operator. This operator (which I haven't picked a symbol for) allows the compiler, when a struct is used where its type is not expected, to *search* the struct for a matching type and use that instead. So if my struct has a `position` struct inside, and `move` expects a `position`, I can prepend my struct with the type resolution operator and expect the compiler to find the `position` member inside my struct. I haven't thought fully about the implications of this, but I think it can be recursive, searching fields themselves for the proper type.
