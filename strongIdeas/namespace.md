# Namespace and Function Dispatch Demonstration

This example demonstrates how namespaces, snake-case declarations, and function dispatch work in this language.

---

## 1. Namespace Separator

The `_` character acts as a **namespace separator**, allowing hierarchical organization of functions and types.

* Creating arbitrary namespace nesting in a single variable declaration is allowed.
* The compiler automatically generates any necessary namespaces to hold the variable.
* Adding to a namespace is permissive: any snake-case declaration will create and add to the required namespaces.
* Snake case is recommended for grouping related identifiers meaningfully.

---

## 2. Namespace Function Definitions

Namespaces can contain multiple functions and act as **callable aliases**.
When a namespace is called like a function, the compiler searches for a matching function signature within that namespace and dispatches it (similar to C++ overload resolution).

```c
namespace print {
    void(int arg) int {
        printf("%d\n", arg);
    }
    
    void(char* arg) string {
        printf("%s\n", arg);
    }
};
```

* Here, `print` contains two functions: one for integers, one for strings.
* The call `print(10)` will dispatch the `int` version.
* The call `print("hello!")` will dispatch the `char*` version.

---

## 3. Adding Functions to Namespaces

You can also **add functions to a namespace** outside of its initial declaration:

```c
void(int arg) print_int {
    printf("%d\n", arg);
}

void(char* arg) print_string {
    printf("%s\n", arg);
}
```

The compiler will resolve these when you call the namespace:

```c
int() main {
    print(10);
    print("hello!");
}
```

* If multiple matching signatures exist, the compiler errors at the **callsite**.
* Conflicting definitions elsewhere are only a problem if they collide with an actual call.

---

## 4. Creating Class-Like Ergonomics with Namespaces

Namespaces can also provide **class-like behavior**:

```c
namespace object {
    struct {...} t;

    t() constructor {
        return ...;
    }
}

// Usage:
object_t myObj = object();
```

* Here, `object` acts like a factory for `object_t`.
* The `constructor` function returns an instance of the object.
* Namespace dispatch rules still apply: calling `object()` invokes the `constructor`.

---

### Key Points

1. Namespaces are primarily **organizational and dispatch mechanisms**.
2. Snake-case grouping is flexible and permissive.
3. Function dispatch within a namespace happens at the **callsite** based on signature matching.
4. You can simulate **class constructors** and other object-oriented ergonomics using namespaces and structured declarations.

---

