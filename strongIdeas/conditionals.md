All compound statements are conditional. All control flow is expressed using code blocks, with an operator for applying control flow to them.

`?` can go before or after a block, and it has two arguments: an expression and a compound statement. They can be called the pre-check and the post-check. One of its arguments *must* be a non-block expression, and the other *must* be a block expression. Compound statements yield a boolean value, but they are not considered expressions for the pre- and post- check operators.

The pre-check defines that each time the block runs, it should apply its lhs precheck. The expression in lhs will be fully evaluated. `?` has only slightly higher precedence than `=`, meaning it can capture entire expressions and evaluate them without parentheses. Parentheses are still preferred. If the pre-check fails, the entire block is skipped.

The post-check defines that each time the block reaches its end via normal means or `continue`, the rhs expression will be evaluated in the same fashion as the pre-check. This defines whether the block will run again, starting at the pre-check.

There is the minor contention that programmers familiar with ternaries might adjust slowly to this. I am not a fan of ternaries in general, and I do not think they are common enough that this should be a major issue.

```
    (true) ? {
        printf("hello world!\n");
    } ? false // the default case; what the compiler assumes if you exclude checks.

    int i = 0;
    i < 10 ? {
        printf("%d\n", i);
    } ? i++ || true // a for/while loop (using a slight hack in the post-check)

    int i = 0;
    {
        printf("%d\n", i);
    } ? i++ < 10 // a do-while loop
```

Compound statement blocks evaluate to a boolean, and they are valid in expressions. The value they evaluate to depends on how they exited.

If they exited because their pre-check failed, the expression evaluates to `0`.

If they exited because their post-check failed or because of a `break`, the expression evaluates to `1`.

When compound statements are used in the `if` pattern ([precheck] {statements} [false]), you can use this to create `else` logic.

```c
    int prechecksuccess = (x < 10) ? {printf("x < 10\n")};
    (prechecksuccess == 0) ? {printf("x >= 10\n")};
```

This is not a very natural `else` pattern. Luckily, the existing rules enable a much better one.

A boolean expression escapes early when its value is definite, or when the rest of the expression can no longer affect the result.

For example, `true || someFunctionWithSideEffects()` never runs the function, because `true || x` is always `true`.

Therefore, `<block> || <block>` only runs the second block if the first one failed its pre-check.

```c
    (x < 10) ? {
        printf("x < 10\n");
    } 
    || {
        printf("x >= 10\n")
    }
```

The main drawback of this is that it takes getting used to, but I think it's an elegant and compositional way to handle control flow. I also think that with some syntax highlighting and experience reading/writing this language, it will not cost any cognitive overhead.

`with` is a keyword that allows one statement or compound statement to continue with the scope declared inside another.

```
    with {int x = 0} {printf("x + 1: %d\n", x + 1)}
```

This helps with loops, as it is often useful to set up a scope for them before they run.

While I think it is elegant, it may not be ergonomic to have the conditions for a loop spread across its body. There is therefore a pattern for defining the control flow and then applying it to a block.

```
    ( conditional block ): {...}
```

or less generally,

```
    ((true) ? {} ? (false)): {...}
```

applies the control flow shape of the conditional block in parentheses to the one on the right of the colon. Statements inside the braces of the template are applied to the body as an increment.

```
    with int i = 0; (i < 10 ? {i++} ? true): {printf("i: %d\n", i)}
```

The goal is not necessarily to create a new, ergonomic way to write control flow. Instead, it is to make control flow a compositional property that follows from simple rules.
