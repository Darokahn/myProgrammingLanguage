# `reentrant` blocks are compiler-managed jump tables with compiler-managed resume-point enumerations.

`reentrant` is a semantic label to a block that changes its behavior, like `while` and `for`. It takes an address to an integer as an argument. When the block is entered, that integer will be matched to enter into the block at a given spot. If no state is held so far, the pointed-to value should be initialized to zero.

When compiling a reentrant block, the compiler analyzes yields and other breakpoints to determine which points of execution are entrance and exit points. It generates an enumeration and a switch statement for entering and exiting the block while respecting and properly writing to the jump-point address. The details of this enumeration and dispatch can be implementation-defined.

```
    void(ref[int] x) stateMachine = ${
        // at this point in execution, an internal jump table will be consulted to use the value of `x` and jump to the next label.
        reentrant(x) {
            ... some code
        }
        // at this point in execution, `x` contains the value necessary to resume where the reentrant block left off.
    }
```

When the block is exited, the proper matched enumeration is written into the pointer so that if it was entered again immediately, it would re-enter right after the last-executed line.

- side note: The argument for asking the user to manage an integer enumeration, whose value is derived by processes that are not visible to the programmer and should be treated as undefined, is simply that the language wants to store the value somewhere and does not want to invisibly decide where. This "indecent exposure" could be solved with a builtin type that is less willing to present itself as an integer. `reentrant` could, under strict compilation options, only take a type `integralIdentity` (or similar) which is not usable as an integer in expressions. A type which is an integral identity but cannot participate in arithmetic could be generically useful.

This language provides a strong interface for using a struct's members as the locals for a function, so it is trivial to write a function with a fully tracked state.

Functions can return a `blockstatus` type, which is a `bool`. Reentrant blocks have a contract with two types of function: One with functions that return `blockstatus` solely, and one with functions that return a struct containing `blockstatus` as their *first* member.

That contract is to virtually strip the signature, removing the `blockstatus` variable from it. Instead, the `reentrant` block will handle the implicit manual process: check whether `blockstatus` is false (would not block), break back out if not. Functions returning `blockstatus` instead return `void`; structs can be used in assignment (Go-style) without the `blockstatus` field present.

