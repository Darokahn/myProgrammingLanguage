# `?`, being free in any language I create (not a fan of ternaries---see below), can be a valid name character.

This enables the most obvious naming convention ever, and I don't know why nobody has ever done this before: boolean variables can have a `?` at the end.

(Note: After writing this, I was informed Ruby does something pretty similar)

I'm probably exaggerating this, but naming bool variables is a significant pain point to me, as you have to phrase them as a question, which usually means inserting an awkward to-be verb at the beginning, whether it really fits or not.

This probably doesn't honestly matter, as inter-dialect and especially inter-language codebases are always a pain and this only removes a drop from that bucket, but it may be worth bringing up that using question marks instead of to-be verbs or other signals makes codebases more symmetrical. A famous post alleges that Canadian developers append "Eh" to the end of booleans, as in "isRed, eh?"

`int draw(uint16_t* sprite, bool useTransparency?);`

As for why I'm not a fan of ternaries, I have a nasty habit of using branchless code wherever possible. I prefer to use `if` only to skip code, and many patterns don't really need to skip code; they need to do a single routine with a computed parameter. I don't like ternaries because they *appear* branchless (not using any conventional control flow structures), but may or may not compile to branchless code.
