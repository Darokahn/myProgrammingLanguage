# Nesting can get overbearing.

This is a simple QOL feature that helps users manage the ugliest of nesting messes by telling a new block to inherit the remainder of its scope.

The `*` character can appear where a block would be expected. It must be followed by a newline. It says that whatever is left in the enclosing block instead belongs to the new block.

For example, `namespace` declarations can be placed at the file scope. If you write `namespace x *\n`, the remainder of the file is put in namespace `x`.

```c
namespace x *

int a;
int b;
```

The language introduces coroutines via `reentrant` blocks. You can label any block as reentrant. Often, you want this to apply to the entire body of a function.

Normally, this would require a `reentrant` function's entire body to be nested for no real reason.

```c
    void(ref[int] x) func = ${
        reentrant (x) {
            ...
        }
    }
```

Instead,

```c
    void(ref[int] x) func = ${
        reentrant (x) *
        ...
    }
```

is much nicer.

Nonnesting blocks can be either much more or much less readable depending on context, and I don't think a concrete or satisfying set of rules can be made for separating good contexts from bad. I won't cherrypick constructs which can or can't access them.

```c
    int main() = ${
        while (true) *
        printf("hello, world!\n"); // This use case is defensible if programmers become familiar with the idiom.
    }
```

```c
    int main() = ${
        if (condition) *
        doSomething();
        moreStuff();
        andMore();
        // You might be tempted to add an `else` for some reason; however, it is syntactically impossible to associate an `else` with the above `if`.
        // This is an example of a case I could probably forbid, but I won't.
    }
```

It is generally a matter of taste and must be up to the programmer to use them properly.
