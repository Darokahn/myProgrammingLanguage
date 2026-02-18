~This document is the old version of a more refined idea, which is under the same name in ../strongIdeas

# C pointers are kind of broken, and C++ adds gas to the fire.

C has a minor issue in that int* could refer to one int, or it could refer to the first of several. Pointers are read heads into the vast void of memory, and we derive different ways of thinking about them by using discipline.

There are three ways in which memory is referenced: As arrays, which are static memory spans that take up space a known offset from the beginning of a data structure (stack, struct, etc.), singular pointers, which are references to single values, and plural pointers, which are references to a sequence of values.

The first use case is differentiated in C, while the second and third rely on the programmer to understand the difference. It's typically trivial, but adds cognitive overhead nonetheless.

C++ adds references, but in an effort to keep backward compatibility, does not clean up pointer semantics whatsoever. Pointers can still be used singularly.

The ideal trifecta is that of references, with similar rules to C++ (perhaps more relaxed), sequences (pointers to remote sequential data), and arrays (pointers to static data with reserved space in a given data structure). Arithmetic should be valid only on sequences. They could also be called roaming pointers. My language uses all three of these as separate entities. They also have a new syntax which may be divisive, but I am quite partial to:

```
    arr[5, int] array; // reserves space in the data structure and lives as a pointer to it.
    seq[int] sequence = ... // some way of getting an address: malloc() or &array; points to the beginning of an array.
    ref[int] reference = &x; // points to the given address. Doesn't cling to C++ alias rules; behaves more like an arithmetic-restricted C pointer.
```

Symbol-heavy syntax can be cool, but I think using words for core types is probably better than using punctuation.

I prefer `arr[5, int]` over `arr[int, 5]` because it reads more naturally as "array of 5 ints".

The square brackets were chosen because the type of `arr | seq | ref` should be specified in an enclosed expression, and parentheses would look too similar to function calls. Angle brackets would also have been an option, but I just personally find them ugly in code.

I might also advocate for the feature that the brackets are optional, because a majority of cases are non-ambiguous:

`ref int x = &y` is no less clear and somewhat more ergonomic than `ref[int] x = &y`
`seq int x = malloc(...);` is similarly okay
`arr 5, int x;` is borderline less readable, but the square brackets are of course always an option.

Eliminating the strict requirement for square brackets lets more complex types be a little nicer on the eyes. The #1 factor in how complex an expression looks tends to be nesting parentheses and brackets. Trimming those down when possible is probably ideal. It could very well be treated similarly to how single-statement-after-if is in some C styles: Use when the case is obvious, but be diligent and abandon it fully when things start to look complex.

address literals have some nuanced rules:

`&value` is a reference address literal and can be assigned to either a sequence or a reference.

If the expression includes arithmetic, this promotes it to a sequence and makes it illegal to assign to a reference type variable.

`arr` type is a static anchor to its contents, and it therefore cannot be lvalue in an assignment.

`seq[]` is equivalent to `void*`.

`arr[0, T]` is valid and may be used to support FAMs.

references to sequences, and generally nesting of all three types, is legal.

type punning through casts is allowed.

This system might decay into bureaucracy in some cases (cough cough rust), in which case `sequence` can be used pretty much exactly how C pointers can. When in doubt, it's always legal to convert any address type to any type of sequence through casting and achieve some of the UB-dependent tricks we sometimes need to use.

There may be reason to borrow C++ rules about nullptr and references, namely that a reference cannot be to null and that references cannot be reassigned. I don't want to engage in hubris, but I have never encountered the fabled bugs of this kind. I've programmed in C for a while and run into all sorts of memory errors, but never the sort where a pointer is NULL where I don't expect it to be. Maybe I just need experience, but I don't think this rule is necessary. I think the rule about C++ references being non-reassignable comes from a place of ergonomic compromise rather than safety. When references can be assigned to non-addresses, it's hard to actually allow reassignment in a way that doesn't introduce subtle bugs. C++ references essentially say "don't worry sweetheart, I promise this isn't a pointer" and create questionable ergonomics in the process. This language will explicitly acknowledge references as what they are: pointers that don't move unless they have a valid new target. A ref cannot be assigned to a non-address, it requires the rvalue to be an address type. This can be another reference, an address literal, or an `arr | seq` cast properly.

`checked` references may be a good way to address generally unsafe operations that cost runtime to protect against.

A `checked` reference opts in to null checks at runtime. If a variable is checked, assigning to it must be wrapped in the `check` construct: `check(expression) {fail path}`

A `checked` reference must test its success on each new assignment. The failure path should guarantee that no path dereferencing the reference is subsequently executed. It may be simpler to say that backwards, that a dereference must prove it can only happen after a successful check. The compiler will analyze the function to determine whether this is the case and error when it is not.

checked ref enforces that:

- Assignments to it are never null. Any assignment that could fail must be wrapped in a check(expression) { fail path }. An assignment could fail if the expression returns a non-checked ref.

- Dereferences are illegal unless they are downstream from a successful assignment or checked themselves. This ensures that a checked ref is never used while null.

- Function arguments of type checked ref are automatically trusted by the callee. The caller is responsible for ensuring the reference is valid.

- Assignments to another trusted checked ref (i.e., one known to be valid via a successful assignment) do not require additional checks.

The failure path of a check should guarantee that no subsequent path uses the reference as if the operation succeeded. All checked references must follow these rules to maintain runtime safety while allowing the compiler to reason about control flow efficiently.

```
    checked ref[int] r;    // Declare a checked reference to int

    // Attempt to allocate memory
    check(r = (ref int)malloc(sizeof(int))) {
        // Fail path: malloc returned null
        // Must handle failure explicitly; e.g., exit or return
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    // Success path: r is guaranteed non-null
    *r = 42;
    printf("Allocated and initialized value: %d\n", *r);
```

If there are further questions, they can likely be answered by reasoning, remembering that `arr` is essentially the C array, `ref` is the C pointer with restricted rules, and `seq` is the C pointer with almost no alterations beyond semantics.

