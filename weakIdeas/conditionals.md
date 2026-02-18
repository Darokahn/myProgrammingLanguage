All compound statements are conditional. All control flow is expressed using code blocks prefixed and postfixed with continuation checks. A block can be preceded and followed by an expression inside square brackets. The expression preceding the block will be evaluated for truthiness to determine whether the block should run. The expression following the block will be evaluated to determine whether it should run again. If it does run again, the pre-check will also be run once more.

The check goes inside square brackets and represents an expression which will be evaluated for truthiness, and determine (for a post- or pre- check respectively) whether the block should run, and whether it should run again.

Break and continue statements apply to the code block they are placed in. `break` will skip all checks while `continue` will skip to the next check.

```
    [true] {
        printf("hello world!\n");
    }[false] // the default case; what the compiler assumes if you exclude checks.

    int i = 0;
    [i < 10] {
        printf("%d\n", i);
    }[i++ || true] // a for/while loop (using a slight hack in the post-check)

    int i = 0;
    {
        printf("%d\n", i);
    }[i++ < 10] // a do-while loop
```

compound statement blocks evaluate to a boolean, and they are valid in expressions. The value they evaluate to depends on how they exited.

If they exited because their pre-check failed, the expression evaluates to `0`.

If they exited because their post-check failed or because of a `break`, the expression evaluates to `1`.

When compound statements are used in the `if` pattern ([precheck] {statements} [false]), you can use this to create `else` logic.

```c
    int prechecksuccess = [x < 10] {printf("x < 10\n")};
    [prechecksuccess == 0] {printf("x >= 10\n")};
```

This is not a very natural `else` pattern. Luckily, the existing rules enable a much better one.

A boolean expression escapes early when its value is definite, or when the rest of the statement can no longer affect the result.

For example, `true || someFunctionWithSideEffects()` never runs the function, because `true || x` is always `true`.

Therefore, `<block> || <block>` only runs the second block if the first one failed its pre-check.

```c
    [x < 10] {
        printf("x < 10\n");
    } || {
        printf("x >= 10\n")
    }
```

The main drawback of this is that it takes getting used to, but I think it's an elegant and compositional way to handle control flow. I also think that with some syntax highlighting and experience reading/writing this language, it will not cost any cognitive overhead.

However, it may be useful to allow nullable identifiers, functioning like how `#define identifier` with nothing after it works in C. Then, programmers are allowed to define style guides where common control flow signal words are used just to annotate code.

```c
    nullword if;
    nullword while;

    if [x == true] {
    };

    while [x == true] {
    } [true];
```

IDEs, or possibly even compilers, could support this style and make warnings when it is not followed properly.

The goal is not necessarily to create a new, ergonomic way to write control flow. Instead, it is to make control flow a compositional property that follows from simple rules.
