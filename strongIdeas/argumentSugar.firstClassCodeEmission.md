This could be included in the main firstClassCodeEmission document, but it feels somewhat separate.

For context, the basic idea of that document is that function declarations are reshaped to put the fully qualified type before the name, and act as assignments to literals:

`returntype(arguments) function = ${...};`

It's important to note that this never causes runtime code generation.

Function arguments are not a basic syntax; they are sugar for the true basic syntax.

Function parameters are referred to via `%n`, and the argument list is a sugar that expands to, for example:

```c
    int(int a, int b, int c) fn = ${...}
    // translates to
    int() fn = ${
        int a = %0;
        int b = %1;
        int c = %2;
    }
```

If the arguments are used without cast, their type is inferred by how they are used. If there cannot be a type which justifies how they are used, the compiler will throw.

Additionally, their type is decided by the first time they are cast (in a vast majority of cases, this happens in the desugar of the param list).

A function's signature can be fully inferred, including return value. This is generally poor practice, and its type should be annotated for the compiler to check against.

Uniquely, functions can be declared without a type, in which case the compiler will infer.

These rules are to address the fact that, in a fully proper function declaration, the declared function should have a fully qualified type *and* the function literal should be cast to that type. This set of rules allows flexibility, including the option for simple functions to be fully inferred.

It is recommended to have the type of a function annotated in at least one place, so that the compiler can check your intentions. It should be a default warning when a function's type is fully delegated to the compiler.

The rules for which assumptions may be made based on the shape of the declaration are as follows:

### `(declaration) function = (matching typecast) ${};`

- This case allows the compiler to use zero inference and check types completely strictly. Everything else desugars to this after inference.

### `(declaration) function = ${};`

- The function literal is cast to the declaration of the function. It is equivalent to above, but with less typing. This is the recommended form.

### `function = (cast) ${function};`

- Odd form, but also valid and safe.

### `function = ${};`

- Still valid, but types will be inferred by the shape of the function. If it refers to arguments, their types will be inferred. If it returns a value, that type will be inferred. If there is no signature that fits the shape of the function, there will be a compiler error.

The last case is the least recommended approach, but it can be nice if a function is as simple as "add two numbers", and the ceremony of a type declaration and argument names would be excessive. Letting everything be implicit is okay because of the simplicity of the case and encouraged because extra declaration would be little more than noise. There's a bit of a slippery slope here, though, as programmers may be tempted to fit less clear cases into this shape and end up obfuscating their code. One could argue that providing programmers with an extra vessel for obfuscating code is bad design, but I would counter that they are more than capable of doing that without any help.

You can also implicitly enumerate function arguments by omitting the number, consuming one argument for each instance. This is generally not very safe or declarative, but it can be used in the shape above to good effect:

```c
    int(int a, int b, int c) fn = ${...}
    // translates to
    int() fn = ${
        int a = %;
        int b = %;
        int c = %;
    }
```

Trying to use this shorthand across a function is short-sighted, and it should rarely be used outside this format. There are, however, some other ways to use it that are clear enough that they aren't really mistake prone.

```
    add = ${return % + %};
```

It should be clear once the programmer is familiar with the rules that this consumes `%0` and `%1` sequentially. Note that this is not macro-like and does not generate a function which works on both `int` and `float` - the types are inferred once at compile time.

Implicitly enumerated arguments are desugared simply, without considering which branches have run. Each instance is simply preprocessed into `%n`, with `n` being the number of prior enumerated arguments. It does not matter whether that argument had been used explicitly by number, or whether it will be later. Mixing the two is generally inadvisable.

These rules allow high flexibility when defining lambdas with low verbosity. One can even define and call a function inside an expression, without ever casting it.

The rules for inferring types could be complicated, or they could be quite simple. Archaic C would infer undeclared function return and arguments as `int`, and this works fine for my language. However, I could use a sophisticated pattern matcher to consider every type that might be usable there (collecting every field access and operator used on it and checking if there is a type which accomodates all of those), and use the first one I find. This is overall more complicated and less predictable, and it's trivial to annotate a function with your intentions, so I think `int` default works just fine.

A declaration can be either missing or incomplete, but the return type is a minimum for a non-missing declaration. You can omit any number of parameters, though there should be no reason to annotate some parameters and not the rest.

The valid declaration shapes are:

- missing
- `returntype()` (parentheses necessary to annotate callability)
- `returntype(params)`

but not:
- `(params)` without return type.
