All control flow is expressed using code blocks prefixed and postfixed with continuation checks. A block can be preceded and followed by an expression inside square brackets. The expression preceding the block will be evaluated for truthiness to determine whether the block should run. The expression following the block will be evaluated to determine whether it should run again. If it does run again, the pre-check will also be run once more.

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
