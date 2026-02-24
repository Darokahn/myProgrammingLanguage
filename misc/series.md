# Many language constructs enable the programmer to specify lists of items in sequence. The `series` comptime construct attempts to unify these and expand their genericity.

The major cases that stand out are function arguments and compound literals. Both function argument callsites and compound literals are collections of comma-separated items.

The comptime construct `series` is a device which adds ergonomics to specifying a list of objects, while applying these ergonomics generically.

## The `,` operator adds the rhs to the lhs series, creating a series containing only lhs if necessary.

A series is a comptime construct that can be consumed by a variety of contexts. One example is that functions can consume a series, ensure that its contents can match the signature of the param list, and then be called.

In fact, functions are always called when written, unless their address is taken. Parentheses are not necessary. In many cases, style should demand them anyway.

Compound literals are similar. An assignment can interpret them and ensure they are the proper shape.

```
    func 1, 2, 3; // calls `func` with the comptime series `(1, 2, 3)`

    arr[4, int] myArr = 1, 2, 3;
```

Parentheses are used to reduce ambiguity in series literals especially when nesting series. In fact, since compound statements are themselves series of statements, the edge for ambiguity is large.

The difference is that `;` is capable of adding statements to a series, while `,` can only add expressions. Since many statements are just expressions, commas are often sufficient.

Curly braces and parentheses also have a kind of duality which interacts with the different delimiters. In the presence of a curly brace, a semicolon will add a statement to the nearest-nested one. This makes it the only way to end a statement with an unenclosed series in it.

## When a series is enclosed in parentheses or curly braces, a newline

```
    arr[4, int] = (
        1
        2
        3
        4
    );
```

String literals compile to a series, as is typically the convention for languages.

A semicolon concludes a statement, and adds it to the series of statements in a block. A comma, and even a newline, will suffice for the same task

Series that this document is currently overlooking because I haven't fully resolved them:

- struct definitions are series of declarations. So far, I have only thought about series of expressions.
- function parameters (in contrast to arguments) are series of declarations.
- compound statements are series of statements.

Ideally, I would unify all of these under the series construct, specifying how semicolons contribute. However, I may end up sacrificing at least a few of them.

For helping with this clarification, I will differentiate:

- Series with declarations inside
- Series with control flow statements inside

