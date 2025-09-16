# `_` is NOT a valid name character. Instead, it is the namespace resolver.

    namespace MyNamespace {
        int x = 0;
    }

    int x = MyNamespace_x;

The justification for this choice is mainly hatred for `::`, but there are a few other pros.<br>
    1. natural, legacy-oriented namespace resolution. Many old libraries already kept namespaces clear by prefixing, such as `SDL_init`.<br>
    2. disable programmingCase arguments. Use camelCase!<br>
    3. disable hacky name collision solutions. Never declare `int _x` again!<br>
Of course, there are the obvious cons:<br>
    1. rejection of an age-old convention<br>
    2. disruption of muscle memory<br>
But <farquaad> that is a sacrifice I am willing to make. </farquaad>

