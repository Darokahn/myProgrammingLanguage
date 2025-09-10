Composition should be favored over inheritance, but there are no composition languages.
Inheritance can be considered a form of monogamous composition, where an object is bound singly to its parent.
Using some basic patterns, we can map composition into inheritance in a way that does not restrict the flexibility of using composition elsewise.

# A struct can route to any of its members, allowing struct.property access instead of struct.member.property access.

```c
    struct point {
        int x;
        int y;
    };

    struct character {
        route struct point position; // note the keyword `route`
    };

    struct character c;
    c.x = 0;
    c.y = 10;
```

The `route` qualifier signals to the compiler that members of `position` can be accessed directly from a `struct character`.

Via this pattern, `struct character` effectively inherits the members of `point`.

Note that in this case, we would not say that the character *is* a position; We would say they *Have* a position. However, this distinction isn't super important under the routing pattern! You can choose to route either only one member and say the object *is* that member, or route several members and say it *has* all those members.

Assigning a variable of one type to a struct that routes that type will work, allowing the programmer to pass a struct to a function as one of its routed types. This is very simple compiletime polymorphism at almost no cost.
