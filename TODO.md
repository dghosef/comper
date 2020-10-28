# TODO
## Random Stuff
Look up conventions  
make sure code clean  
error checking(find library too)  
Testing(find library too). 
typedefs for measures??
Style needs bass, drum, comping patterns and note rules   
Constants for default function arguments  

## Objects

### Note
#### Public methods
letter name  
octave number  
midi number  
operators  
construct(name = C, octave = 4, number = 60, velocity = 127/2?)  
destruct  

### Chord
#### Public methods
name  
root  
third  
fifth  
seventh  
ninth  
eleven  
thirteen  
operators  
voicing  
setVoicing(voicings, if given multiple, pick one that's closest, root octave)  
construct(name = C) - make sure to have error messages for illegal names  
destruct  

### DrumNote
enum drumPiece(figure out where to place this later)  
#### Public methods
constructor(piece, midiNumber)  
Piece  
midi number  
operators  
destruct  

struct measure - drumMeasure, bassMeasure, chordMeasure

### Progression
#### Public Methods
constructor(string, vec, style)  
vec chords Progression  
int timesig  
int tempo  
map\[voicings, chordPatterns, bassRules, drumRules\]  style  
generateTrack(num = 1)  
numChoruses
#### Private
generateBassLine(end note = C. Has to be in valid range)  
generateChords  
generateDrums  
readStyle(file)  

### Song
songFile should include style and chords for (head, tail, and optionally improv)  
constructor(songFile, numHeads = 1, numTails = 1, numImprov = 3)  
progressions = {progression(songFile head), progression(songFile improv), progression(songFile tail)}  
track = all those progs.genTrack(number specified)  
trackToMidi(filename)
