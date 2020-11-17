# Format for bass line note(simple quarter note only)
- x = the distance traversed. Can be positive or negative. If not included defaults to 1
- u = the unit of distance. Can be 'c' for chord tone(1, 3, 5, or 7), 's' for scale tone(2, 4, 6), 'r' for root, 'l' for leading tone to next root(The fifth above, a semitone before/after the root, or or diatonic before/after next root(typically only notated on last beat of chord or bar), 'lc' for semitone before a chord tone, and 'ls' for semitone befor scale tone.
- y = include if we are at the last beat of a bar/chord

We should have format(pseudoregex) (x?uy?_)*(x?uy?)
## Example
- If we have C-7 - - - | F7 - - - | Bbmaj7 - - - | - - - - || as our chord progression and
- STARTNOTE = C and
- our bass line is STARTNOTE_c_s_l_
