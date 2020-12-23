# Style File Specification

Style File consists of a collection of Context-Free-Grammar-like structure. The general syntax for each structure is
```
[title] % this is a comment
<START> <subject><predicate>_ 100
<subject> He_ 50 | She_ 50
<predicate> went_to_the_<room> 50 | ran_to_the_<room> 50
<room> bathroom<START> 20 | restroom<START> 30
```

Each line must have the general format `<nonterminal> expansion weight | expansion weight | ...` where `nonterminal` can be any letter, number, _, +, or -, `expansion` can be any of those and can also include other nonterminals enclosed in `<>`, and `weight` can be any integer. Each such structure is used to generate a string. Generation always starts at `<START>`. An expansion after `<START>` will be chosen using a weighted random selection with the specified weight after each expansion. If the chosen expansion includes more nonterminals, each of those nonterminals will be expanded using its rule in the next step. Once the CFG has been expanded for the specified number of steps, all remaining nonterminals are deleted from the resulting string.

For example, here is one possible sentence the above structure could generate after 3 expansions 

Step 1: `<subject><predicate>_` This is the only possible expansion it could pick since the `<START>` nonterminal only has 1 option 

Step 2: `He_went_to_the_<room>` This is one possible expansion where we replace `<subject>` with `He_` and `<predicate>` with `went_to_the_<room>`. There is a 25% percent chance of this exact sequence occuring.

Step 3: `He_went_to_the_bathroom<START>` Again, this is just replacing `<room>` with one of the 2 options. Note that there is a 20 / 50 or 40% chance we pick `bathroom<START>` and a 30/50 or 60% chance we pick `restroom<START>`.

At this point, if we are finished with generation, we discard the `<START>` and our final product is `He_went_to_the_bathroom`. If we were going to continue with generation, we would go back to `<START>` and continue the process. \
Note that recursion is possible. For example, the following would just generate a bunch of `j`s
```
<START> j<START> 100
```
The style file must contain 4 such structures titled `bassPattern`, `bassDirection`, `compingRhythm`, and `compingDirection`

`bassPattern` must generate a string with any combination of the characters `0`, `S`, `C`, `A`, `F`, `O`, and `R`. The length of the string after `n` steps must be at least `n` characters. Any extra characters will be ignored. The walking bassline generation starts by generating a string from bassPattern. It sequentially reads through the generated string and uses it to generate the next notes in the bassline. If it reads an `S`, it plays the next note in the current chord's scale. If it reads an `A` or `C` it plays the next chord tone. If it reads a `0`, it plays the chord root. If it reads an `F`, it jumps over 1 chord tone and plays the 2nd closest chord tone. If it reads an `O` it jumps by 1 octave in the specified direction. If it reads an `R`, it repeats the previous note. The program will continue reading this pattern until it gets to the last note of the chord, at which point it will play a leading note to the next chord and repeat the process for the next chord

`bassDirection` must generate a string with any combination of the characters `U` and `D`. The length of the string after `n` steps must be at least `n` characters. Any extra characters will be ignored. For each chord, a direction string is generated and looped over to determine the direction of each note in the bassline where each character's direction determines its corresponding note's direction. For example, if the string `UUDU` is generated, the bassline will move up in pitch for the first 2 notes, the 3rd note will move down, and the 4th will go back up. Direction will get overriden if the bass player has gone too low or too high

`compingRhythm` must generate a string with any combination of the characters `q`, `e`, `8`, `4`, `2`, `1`. `q` stands for quarter note rest, `e` is eighth note rest, `8`, `4`, `2`, and `1` stand for eighth note, quarter note, half note, and whole note respectively. 4 beats worth must be generated every step. The generated rhythm is the rhythm that the automated chord playing will play. The rhythm is generated once for the whole song. 

`compingDirection` has the same specifications as `bassPattern` including characters generated/step, but its generated direction determine the direction of the leading note of the chord rather than the direction of the bassline.
