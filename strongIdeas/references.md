~This is the evolution of a prior idea, which has been moved into the weakIdeas directory.~

# Unified Pointer Type System

## Core Concept

Pointers can be unified into three main use cases, but the ruleset we apply can be plotted as a full matrix of 8 different pointer types.

The three pointer types you really need are **singular**, **plural**, and **array**:

- **Singular** is like C++ reference: A pointer to just one value, with no ambiguity as to whether it is a pointer to the beginning of several values.
- **Plural** is like a raw C pointer, allowing indexing and arithmetic.
- **Array** is like a C array, allocating its space on the stack and being immutable (not a valid lvalue). There is no reasonable or ergonomic way to decouple statically allocating memory in a data structure from acquiring that memory via a named variable, so this type must have a place.

## Compositional Rules

Any pointer type can be composed from these orthogonal properties:

- `mut | immut` - whether it can be an lvalue after initialization
- `singular | plural` - whether indexing and arithmetic are allowed. The `plural` qualifier also takes a compile-time integer bound.
- `embedded | indirect` - whether it automatically takes up memory on the data structure it belongs to and is assigned to point to that memory

Some of the addr types you can construct from this aren't very safe and others aren't very useful, but all of them make sense.

## Syntax

The declaration for a pointer type is, for example:

`immut plural[5] embedded addr[int] x;`


Just `addr` is `void*`.

## The Three Sugared Types

That's a lot to type, and comes with overhead. These three cases have sugar:

- `ref[T]` = `immut singular indirect addr[T]`
- `seq[T]` = `mut plural[] indirect addr[int]`
- `arr[n, T]` = `immut plural[n] embedded addr[T]`

The default for each qualifier is `mut`, `singular`, and `dynamic`. This makes `addr[T]` technically sugar for reassignable singular references, but only because of the defaults.

The 4 non-sugared address types would be accessible to the programmer, but the language would recommend using the core 4 sugared ones.

## Special Cases and Edge Cases

By covering every corner of a 2x2x2 matrix, this system encounters a little bit of "you can do that, but why?" I generally find this acceptable, and at the very worst, a compiler warning can flag usages that probably shouldn't happen.

### `mut embedded`

The `mut embedded` combination is generally nonsense as we would rarely want to leave a stack allocation behind by reassigning the variable that points to it, but I don't want to take away the freedom. It allows walkable stack arrays without declaring a new pointer that aliases the stack allocation, and maybe some other tricks.

### Plural with Bounds

The value in brackets after plural is *generally* only used by embedded variables, but it can be used to declare intent for an indirect plural address. `sizeof` responds to that value, and it may allow enforcement of certain static bounds checks at compile time. Not radically useful, but applicable sometimes.

### Embedded Semantics

`embedded` means that the memory specified by the declaration will be created either on the stack, in the data segment, or in the struct it's declared inside.

The combination of `embedded` and `immut` means that the value will always point to the same memory location, and that that location is a known offset from its owner data structure (stack, struct, DATA). This should mean that the compiler is free to optimize out actual runtime storage of the value and save it only as a compiletime symbol.

My language actually *requires* this. The standard says that `embedded immut` has no address, and must always be a compiletime symbol.

Attempting to take the address of `embedded immut` is illegal.

## Initialization vs. Assignment

This type system allows different rules for initialization vs. assignment. The initialization of immut is legal, but assignment is not. The initialization of embedded expects the type it derefs to or a series containing that type, while the initialization of indirect expects an address.

## Auto-Dereference for `immut singular`

Assigning to `immut singular` is generally illegal, so we use this deadzone to treat *usage* of `immut singular` as implicit dereference, like how C++ does. `immut singular indirect addr[int] x = &something` is later usable as just `x = 10`, auto-dereferencing.

### The Fundamental Insight

`immut singular embedded addr[int]` means exactly the same thing as `int`.

Since `embedded` initialization expects the type it dereferences to and will allocate space for that value on the stack, and because `immut singular` dereferences automatically, using `immut singular embedded addr[T]` has zero differences from using just `T`.

**This lets the language say: "all variables are addresses. Literal types only exist temporarily inside expressions."**

## Desugaring Rules

`immut singular` is auto-deref-ed via simple desugaring:

- `x + 1` desugars to `*x + 1` (auto-deref)
- `sizeof x` desugars to `sizeof *x` (size of the deref type)
- `&x` is `&*x` (using `&` just cancels out the auto-deref)

This desugaring makes everything consistent and predictable with zero runtime cost.

Here is a write-up of each pointer type, whether it's sugared, and whether it's ever useful.

## immut singular embedded addr[T]
- The universal value type, all `T` desugars to this.
## immut singular indirect addr[T]
- Non-reassignable reference, much like C++.
- `ref[T]` desugars to this.
## immut plural[n] embedded addr[T]
- Good old stack allocated array.
- `arr[n, T]` desugars to this.
## immut plural[n] indirect addr[T]
- Address of some value, assumed to be plural, but not enforced.
- Good to use as a slice or view of an `arr` or `sequence`.
## mut singular embedded addr[T]
- Address of an int that is placed on the stack. Can be reassigned. Not super obviously useful.
- Can be used for shorter code when something is *either* a default value *or* a reference to incoming param.
- Note `mut singular`, not `immut singular`, so it's not auto-deref-ed and not incredibly ergonomic to use as a reference.
- Hard to defend its use case, but it might be useful when you would do:
```
    int x = 10;
    func(&x);
```
instead:
```
    mut singular embedded addr[int] x = 10;
    func(x);
```
There's a semi-common pattern where several functions which need to be called in a row take `T*` and expect it to point to local storage, so you end up using the address of your variable more often than you actually use the variable. That's where you might create a pointer that's an alias to your variable, so you don't have to keep taking its address. This type both stores your value and exists as a pointer to it at the same time.
## mut singular indirect addr[T]
- Reassignable reference to any `T`. Pretty useful.
## mut plural[n] embedded addr[T]
- Walkable stack allocated array. Can be used to iterate over stack data without using an index or a new pointer alias. Can also be used to swap stack arrays.
## mut plural[n] indirect addr[T]
- Freest pointer type. Used for pretty much anything if other rules get in the way. Especially useful for tracking a dynamic sequence of elements that may move around memory, I.E. via realloc.
- `seq[T]` desugars to this.
## addr
- `void*`. Can only be rvalue for assignment to another pointer type.

I think the proof of this system and its validity is that only one of the pointer types can really be written off as unuseful. `mut singular embedded addr[T]` isn't obviously useful for anything, but the rest actually are. And even that one might not be written off completely, as there's no saying it can't have its utility found in some corner of some codebase.

Note that `mut + embedded` is by far the least useful combination, with the most niche cases. But note also that it does something subtle to compilation and static analysis. `immut + embedded` pretty much grants the compiler executive permission to turn that address into a compiletime symbol. But `mut + embedded` requires the compiler to store a real variable with a reference to the allocated segment, with a bit of ambiguity as to where. In a struct, that could lead to some unexpected results. The definition for this should be that the pointer is allocated first, with the memory it points to going at the next properly padded address.

I think the cognitive load is negligible, because the language only truly asks the user to engage with the core 3 sugared types. Everything is possible with just those 3, and knowing about the intricacies of the rest is the precursor to even wanting to use them. When using other types, they should probably be typedef'd to user-defined sugar as well. People who understand them can easily write libraries so people who don't want to engage with their inner workings can still use them.

