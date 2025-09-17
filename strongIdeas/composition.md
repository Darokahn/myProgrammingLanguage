Composition should be favored over inheritance, but there are no composition languages.
Inheritance can be considered a form of monogamous composition, where an object is bound singly to its parent.
Using some basic patterns, we can map composition into inheritance in a way that does not restrict the flexibility of using composition elsewise.

Note that this document does not focus on object oriented method calling, but it is compatible with it. See state.md for my notes on object-oriented state and method calls.

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

# A struct can substitute to any of its members, allowing it to be implicitly converted to that type via substitution.

```c
    struct point {
        int x;
        int y;
    };

    void printPoint(struct point p) {
        printf("%d, %d\n", p.x, p.y);
    }

    struct character {
        substitute struct point position; // note the keyword `substitute`
    };

    struct character c;
    printPoint(c); // works, because `substitute` tells the compiler `struct character` can be implicitly converted to its inner member with type point.
```

If the function takes a pointer, but the inner substituted struct is stored as a complete struct rather than a pointer, then a pointer to the substituting struct may be coerced into a pointer to the substituted struct.

To restate that potentially unclear statement:

if function takes type `struct a` and my struct contains one substituted `struct a`:  
- passing my struct to the function will implicitly use the substituted struct a.

if function takes type `struct a*` and my struct contains `struct a`:  
- passing my struct to the function will not implicitly use the substituted struct a.
- passing the address of my struct to the function will implicitly use the address of the substituted struct a.

```c
    void printPoint(struct point p) {
        printf("%d, %d\n", p.x, p.y);
    }

    void move(struct point* p, int x, int y) {
        p.x += x;
        p.y += y;
    }

    struct character c;
    c.x = 0;
    c.y = 0;

    printPoint(c);

    move(&c, 10, 10);

```

# The `delegate` keyword can be used to mean "both route and substitute"

In cases of conflicts introduced by routing (several members contain conflicting names) or by substitution (several members of the same type volunteer to be substituted), the early bird gets the worm.

In these cases, the first declared will win the conflict. The compiler should also warn about this. As for how to mute these warnings when the priority behavior is acknowledged and intended, there are a few options.


1. for substitution, it is generally nonsense to substitute two different members of the same type. The way to remove this warning should simply be to remove the `substitute` declaration from the second member.
2. for routing, a proposed solution is "route mapping (see below)," although I am hesitant to introduce this kind of extra ceremony.
3. for routing, a `prior` qualifier may be a decent solution; Acknowledging that the members of this member will be used before any others.

route mapping would look something like this:

following the `route` keyword, you can optionally provide a map from one name to another. You can also choose to ignore any names.

```c
    #if ROUTE_STRATEGY == REMAP
    struct character { // to alias routes for the second object
        route struct point position;
        route {x : velocityX; y : velocityY;} struct point velocity;
    };

    #elif ROUTE_STRATEGY == IGNORE
    struct person {
        route struct skin mySkin;
        route {ignore color;} struct hair myHair;
        route {ignore color;} struct eyes myEyes;
    };
    #endif
```

Using `prior` would have specific rules. Its sole use case would essentially be to acknowledge and silence warnings about shadowing.

If two or more members have an inner member with the same name, the first declared will *always* get priority. This cannot be changed.
The `prior` qualifier would *only* be valid on this member, not on any subsequent ones. It would simply keep the programmer accountable and intentional when it comes to shadowing.

This would also apply to first-class member names. They would need to be declared before any routed struct members that shadow them, with the `prior` keyword, to avoid an error. Unhandled first-class member shadowing would cause an error because shadowing them would make them inaccessible.

It may be best practice to tolerate the warnings about shadowing until the layout of a struct is finalized, to avoid silencing them even for future additions.

Additionally, the *best* solution to all of these conflicts is of course to rarely run into them. Routing should be used sparingly, as many times it is perfectly tolerable to access subfields. Core functionality should be routed up, while things that have to do with the details of functionality should stay associated with those details. Writing code this way should minimize cases of conflict.
