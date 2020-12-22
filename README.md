# Comper

Comper is a backing track generator for jazz songs.

## Building

Build by making a build directory(i.e. `build/`), run `qmake` in that directory, and then use `make` to build the desired target. An executable named `comper` will be generated

Example:

```bash
mkdir build && cd build
qmake ..
make
```

## Usage
General usage of comper is of the form `comper <style file> <progression file> <bpm>` where `<style file>` is the path to the style file, `<progression file>` is a path to the progression file, and `bpm` is an integer representing the beats per minute of the song.

## File formats
### Style File
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

Step 2: `He_went_to_the_<room>` This is one possible expansion where we replace `<subject>` with `He_` and <predicate> with `went_to_the_<room>`. There is a 25% percent chance of this exact sequence occuring.

Step 3: `He_went_to_the_bathroom<START>` Again, this is just replacing `<room>` with one of the 2 options. Note that there is a 20 / 50 or 40% chance we pick `bathroom<START>` and a 30/50 or 60% chance we pick `restroom<START>`.

At this point, if we are finished with generation, we discard the `<START>` and our final product is `He_went_to_the_bathroom`. If we were going to continue with generation, we would go back to `<START>` and continue the process. \
Note that recursion is possible. For example, the following would just generate a bunch of `j`s
```
<START> j<START> 100
```
