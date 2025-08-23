All control flow is expressed using code blocks prefixed and postfixed with continuation checks. A block can be preceded and followed by an expression inside square brackets. The expression preceding the block will be evaluated for truthiness to determine whether the block should run. The expression following the block will be evaluated to determine whether it should run again. If it does run again, the pre-check will also be run once more.

A code block is interchangeable for a statement. Any code block or statement can be prefixed and postfixed with a check.

The check goes inside square brackets and represents an expression which will be evaluated for truthiness, and determine (for a post- or pre- check respectively) whether the block should run, and whether it should run again:
break and continue statements apply to the code block they are placed in. `break` will skip all checks while `continue` will forward to the next check.

```
    [true]{printf("hello world!\n");}[false] // the default case; what the compiler assumes if you exclude checks.
    int i = 0;
    [i < 10]{printf("%d\n", i);}[i++ || true] // a for/while loop (using a slight hack in the post-check)
    int i = 0;
    {printf("%d\n", i);}[i++ < 10] // a do-while loop
```
```
    void(data {int x;} int(int arg)* func, int x) addMaker = ${
        *func = ${
            return ?(x) + arg;
        };
    };

    int(int argc, char* argv[]) main = ${
        {
            [argc < 2] break;
            [strcmp(argv[1], "hello") == 0] {
                printf("argument was hello\n");
            } 
            | {
                printf("argument was not hello\n");
            };
        };
        data {int;} int(int) add1;
        addMaker(1, &add1);
        int i = 0;
        [i < 10] {
            printf("i is %d\n", i);
        } [i = add1(i)];
        return 0;
    };
```
