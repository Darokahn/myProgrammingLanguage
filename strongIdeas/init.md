# Due to the unique reference semantics of this language, the difference between initialization and regular assignment must be formalized.

For clarity and understanding, it will be useful to summarize the different pointer types used by this system. First, their basic properties; then, their more subtle features.

All variables are pointers, conceptually. There are conceptual pointer types that act like value types, and the compiler is allowed to lower them as such.

Pointer types can be created using three orthogonal qualifications: `immut | mut`, `singular | plural`, `embedded | indirect`.

The language provides sugared definitions for common compositions of these qualities.

The form for a declaration, when not using sugared  types, is `immut singular embedded &T myvar;`.

`immut` is the argument deciding whether a pointer can be repointed after initialization. Its pointed-to value can be changed through it, but it cannot point elsewhere.

`singular` is the argument deciding whether you can derive new pointers from a base pointer in an expression; whether arithmetic and indexing are allowed.

`embedded` comes with the most nuance, as most type systems treat it as a special case. `embedded` pointers are the method for allocating automatic storage. The most general way to describe them is that they always allocate enough space to accomodate their declaration inside the data structure they are declared for. This can be file scope (data segment), function local (stack), or struct (wherever the struct is put).

`plural` can take a comptime argument in square brackets, which is an annotation of how many values it should conceptually own. Embedded pointers respect this annotation and allocate memoy accordingly.

There is already a slightly uneasy reasoning surface about initialization; that is, which values do embedded pointers take as initializers? They're technically pointers, so how do I give them an inner value? This will be resolved.

The important subtle feature, and the one that brings this system further out of equilibrium, is the auto-dereference applied to `singular immut` types. This feature is fundamental to the type system and responsible for most of its virtues. Despite how it causes some problems, it is necessary in the language.

`singular immut` types are implicitly dereferenced any time they are use in an expression. The motivation for this is that there is no reason to care about their pointer value beyond initializing what they point to. Its address cannot be changed, and you cannot derive a new pointer from it. There is no reason to use the name of a `singular immut` pointer except to dereference it or pass it elsewhere. In the case of `singular immut`, the `&` address of operator essentially "cancels" that dereference so you can use a reference in an expression without deref (almost exclusively for passing).

`immut singular embedded` is the type of the value-like pointers mentioned earlier. In fact, `immut singular embedded &T` is infinitely equivalent to `T`. You can nest i as many times as you want, and it does not become a new type.

Since singular value-type variables are automatically allocated, they are `embedded`. Obviously, they are `singular`, and most languages do not even hold space for the concept of repointing a variable. They are also `immut`.

With these properties, plus auto dereferencing, `immut singular embedded` is identical to a value type variable in other languages.

All of this comes wth a pretty thorough mangling of the initialization space, though. This is where initialization semantics come in.

All values must be initialized with a pointer type expression; any expression that can be coerced into an address. Initialization-type assignment must always accept a pointer value.

To make this tenet viable, `embedded` storage specifically can accept "embedded expression" values.

The result of an expression does not conceptually have storage. The expression is calculated in ephemeral space, usually by juggling registers and using the stack.

You can coerce the result of an expression into automatic storage via `embed`. This gives it an address which can be taken by a pointer.

`embedded` assignments do this implicitly, so you do not need to say `int x = embed random()`. `embed` as a keyword is mostly a formalization of what `embedded` assignments do.

Conceptually, embedded expressions always occupy automatic storage and have addresses. In practice, the compiler may store them as ephemerally as it sees fit. If an embedded expression is the rhs of an `embedded` assignment, that concept is realized. The value returned by an embedded expression is a pointer.

Compound literals are naturally embedded expressions.

The `init` keyword allows you to target initialization behavior explicitly. `int x = 0;` is equivalent to `int x; init x = 0;`.

For immutable storage, `init` must be proven to run only once, including inside the declaration.
