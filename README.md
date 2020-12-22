# Comper

Comper is a backing track generator for jazz songs.

### Building

Build by making a build directory(i.e. `build/`), run `qmake` in that directory, and then use `make` to build the desired target. An executable named `comper` will be generated

Example:

```bash
mkdir build && cd build
qmake ..
make
```

### Usage
General usage of comper is of the form
`comper <style file> <progression file> <bpm>`
where `<style file>` is the path to the style file, `<progression file>` is a path to the progression file, and `bpm` is an integer representing the beats per minute of the song.
