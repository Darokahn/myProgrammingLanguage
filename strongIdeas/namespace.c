// `_` is the namespace separator
// This actually doesn't cause many problems, because creating arbitrary namespace nesting in a single variable declaration is allowed.
// The compiler simply creates as many namespaces as necessary to support the declaration, and places the variable there.
// When looking up strings,
// This is largely invisible, as you cannot accidentally interact with a namespace. If you didn't know this behavior, you would use it without realizing.
// Adding to a namespace is incredibly permissive, such that any snake case declaration is willing to create and add to any necessary namespaces to support it.
// Snake case users can rest easy, but
// Snake case should be used to group things that meaningfully share a prefix.

// Namespaces are callable as an alias to each of the functions defined inside them.
// The call will search the namespace for a matching signature and dispatch that function at the callsite (C++ rules or similar).
// This can be done explicitly via nesting several declarations inside a namespace.

namespace print {
    void(int arg) int {
        printf("%d\n", arg);
    }
    void (char* arg) string {
        printf("%s\n", arg);
    }
};

// However, it's intended to be done by adding declarations to that namespace via separated declarations.

void(int arg) print_int {
    printf("%d\n", arg);
}

void(char* arg) print_string {
    printf("%s\n", arg);
}

int() main {
    print(10);
    print("hello!");
}

// The compiler should error if it finds several candidates for a given call shape.
// It should do this at the callsite, not wherever the conflicting definition is created.
// This is because matching signatures are not inherently a problem, they are only a problem at the callsite if it exists.

// creating a set of namespace definitions that amount to class ergonomics is also quite easy

namespace object {
    struct t {...};

    t() constructor {
        return ...
    }
}

// figure out why this all works!
object_t myObj = object();

