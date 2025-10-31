# Function arguments are not a basic syntax; they are sugar for the true basic syntax.

For context, this idea expands upon the "First Class Code Emission" document, which unifies functions and function pointers, also giving them more ergonomic syntax.

The document also specifies that the function body and the function declaration are distinct, and each has its own type. The type of each one must match for compilation to be successful, but assigning a function body literal to a function declaration will cast the function body to the proper type.

## Function parameters are referred to via `%n`.

```c
    // uses fully proper declaration, with type on one side and cast on the other.
    int(int, int) func = (int(int, int))${ 
        return %0 + %1;
    };
```

This is obviously not ergonomic, and there is, of course, sugar for using regular parameter names. Let's first consider just the function literal.

```c
    ${
        return a + b;
    };
```

Although it looks like a clear compilation error in most other languages, this function literal is perfectly valid. This is because of the particular rules function literals compile with.

## How Function Literals Compile

When a function literal is evaluated, it follows this process:

1. **Search for a cast**: When a function literal has access to a cast, which can be borrowed from a variable it is being assigned to, it follows a process for type safety. It inserts named declarations of its enumerated parameters that match the ones specified by the casts. For example, a function literal cast to `int(int a)` will insert `int a = %0;` at the beginning of its text. It will then insert the `freezeparams;` statement, which disables enumerated parameters beyond that point. Now, any further names it encounters *must* have been declared by the parameter list. This allows function literals to use named parameters when its cast provides knowledge of them. The function is then done compiling.

2. **If no cast found**: When no cast is available (like the example above), the function literal has a way to recover:

3. **Unknown variables become implicit parameters**: When the compiler encounters a variable that corresponds to no known name for the first time, it automatically treats it as `%n`, where `n` is the number of such variables already encountered. This processes the function literal into a valid generic function. Generic functions must be cast before they are used, but this is allowed to happen implicitly at the callsite. `int x = ${return a + b;}(1, 2)` is valid, albeit not very clear.

4. **Casts validate shape compatibility**: When casting, the compiler checks whether the cast matches the shape of the generic function and binds it to that type if compatible. No insertion of parameter declarations is needed.

The programmer does not need to know about this feature to use the language. Foremost, it is a justification for why function literals do not need an explicit typecast when assigned. Secondarily, it allows useful declaration freedom while maintaining function literal independence.

Uniquely, functions can be declared without a type, in which case the compiler will infer. When a function is declared without an explicit type annotation, it becomes a compile-time generic whose types are assigned and checked at each callsite. This allows the same function definition to work with different types naturally:

```c
    add = ${return %0 + %1};

    main = ${
        int a = add(1, 2);          // instantiates int(int, int) version
        float b = add(0.1f, 0.2f);  // instantiates float(float, float) version
    };
```

When a function has an explicit type annotation, that type is used strictly with no inference. This is the recommended approach for clarity and type safety.

It is recommended to have the type of a function annotated in at least one place, so that the compiler can check your intentions. Generics are okay, but should be kept small.

The rules for which assumptions may be made based on the shape of the declaration are as follows:

### `(declaration) function = (matching typecast) ${};`

- This case allows the compiler to use zero inference and check types completely strictly. Everything else desugars to this after inference.
- The cast is bound directly to the function literal, allowing parameter name desugaring to occur. The cast validates the shape and may inject parameter declarations into the function body if parameter names are used in the literal.

### `(declaration) function = ${};`

- The function literal borrows a cast from the variable it's assigned to, lifting it to the canonical declaration.

### `function = (cast) ${};`

- The function name borrows its type from the cast of the function, lifting it to the canonical declaration.

### `function = ${};`

- The function literal self-compiles to a generic. Types will be inferred by the shape of the function and instantiated as compile-time generics at each callsite. If it refers to arguments, their types will be inferred from usage. If it returns a value, that type will be inferred. If there is no signature that fits the shape of the function at a given callsite, there will be a compiler error.

The last case is the least recommended approach, but it can be nice if a function is as simple as "add two numbers", and the ceremony of a type declaration and argument names would be excessive. Letting everything be implicit is okay because of the simplicity of the case and encouraged because extra declaration would be little more than noise. There's a bit of a slippery slope here, though, as programmers may be tempted to fit less clear cases into this shape and end up obfuscating their code. One could argue that providing programmers with an extra vessel for obfuscating code is bad design, but I would counter that they are more than capable of doing that without any help.

## Implicit Parameter Enumeration

You can implicitly enumerate function arguments by omitting the number, consuming one argument for each instance. This is generally not very safe or declarative, but it can be used effectively for simple cases:

```c
    add = ${return % + %};
```

It should be clear once the programmer is familiar with the rules that this consumes `%0` and `%1` sequentially. When used without explicit type annotations, this creates a compile-time generic that is instantiated at each callsite with the appropriate types.

Implicitly enumerated arguments are desugared simply, without considering which branches will run. Each instance is simply preprocessed into `%n`, with `n` being the number of prior enumerated arguments. It does not matter whether that argument had been used explicitly by number, or whether it will be later. Mixing the two is generally inadvisable.

## Declaration Shapes

These rules allow high flexibility when defining lambdas with low verbosity. One can even define and call a function inside an expression, without ever casting it explicitly.

A regular declaration can be either missing or incomplete, but the return type is a minimum for a non-missing declaration. You can either list all parameters or omit all parameters; it is not allowed to just declare some.

The valid declaration shapes are:

- missing (creates compile-time generic)
- `returntype()` (parentheses necessary to annotate callability)
- `returntype(params)`

but not:
- `(params)` without return type.

It is possible to mix statically typed params with generic ones, although it must be done manually.

```c
    // providing a cast but ommitting names for parameters prevents the compiler from using any name sugar. Undeclared names in the function literal will always fail without a fallback. It inherently prevents insertion of the `freezeparams;` statement, which would otherwise disable enumerated parameters after the compiler inserts definitions.
    int(int, int) function = ${
        int a = %0;
        int b = %1;
        return a + b + %2;
    };

    // With names: int(int a, int b)
    ${
        int a = %0;      // inserted by compiler
        int b = %1;      // inserted by compiler
        freezeparams;    // inserted by compiler - blocks further %n usage
        return a + b + %2;  // ERROR: %2 is frozen
    }

    // Without names: int(int, int)
    ${
        int a = %0;      // manual
        int b = %1;      // manual
        // no freezeparams inserted
        return a + b + %2;  // OK: %2 is still accessible
    }
```

## Type-Safe Generics

If you want type-safe generics, any function can attempt to be explicitly cast to a type with the same number of arguments, even if it already has a concrete type. The compiler will then check whether this cast can be valid and error if not. You can use this to alias one typed function to several other implementations, or cast it explicitly at the callsite.

## Summary

The key insight is that function literals are self-sufficient: they compile to valid generic functions by treating unknown names as implicit parameters. Casts and type annotations validate shape compatibility rather than injecting code across assignments. This makes the system compositional while maintaining ergonomic syntax for both explicit typing and generic inference.

# Design Note: Function Literal Independence

## The Tension

The problem is that the function literal that uses named parameters cannot stand on its own; it requires a cast which reaches across the assignment. The problem isn't *that* the cast reaches across, it's that it's *required*.

When a function literal uses named parameters like `${return a + b;}`, those names only exist because the type declaration injects them into the body. The literal depends on external context.

## The Solution

The rules were refined so function literals can compile independently:
- When encountering an unknown variable for the first time, treat it as `%n`
- This makes the literal a valid generic that can be cast by assignment or at the callsite
- When a cast is available, use it for type-safe desugaring (the common case)

## The Realization

This has essentially been a roundabout justification of the original position, with slightly clearer rules.

The cast reaches across the assignment in pretty much all cases anyway - and that's fine. You should use anonymous parameters for generic functions anyway, and generic functions are the only case where a cast is unavailable.

The fallback mechanism makes function literals theoretically independent, but in practice the original design was already sound. The value is in clarifying the formal semantics and confirming the intuition was correct.
