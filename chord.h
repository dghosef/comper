#ifndef CHORD_H
#define CHORD_H
#include "note.h"
#include <string>
#include <vector>
#include <set>

/**
  * @class Chord
  * @brief stores a chord's notes, duration, octave, etc
  * @author Joseph Tan
  * @details The root must be present at the beginning or after a slash. Each degree will be
  * assumed to be the same as a major scale(ex: the 9th of a C chord is D and its 13th is A)
  * unless specified otherwise. Here are the ways to specify otherwise:
  * a flattened 9 can specify b9, b2. Sharp 9 can be #9 or #2
  * A minor third can be specified by putting m/-/~ after the root(Cm,C~,C-), a lowercase root(c7)
  * specifiying min, dim, or hdim, or specifying b3. A sharp 3 can be specified by #3(I've never
  * seen this in a chord, but I'm sure that someone has done it before). hdim = half diminished
  * Fourths can be altered with #4, b4, #11, or b11.
  * Fifths can be altered with aug, dim, hdim, #5, +, and b5
  * Sixths can be altered with b/#13, b/#6
  * A seventh will be major if maj is present or if the chord name is only a uppercase
  * note(C# == C#maj != c#)
  * Otherwise, it will be a dominant 7. If it has dim and not hdim, it will be a diminished 7
  * b7 will override everythiing except diminished and make it dominant
  * Separate anything that could cause confusion(like Gb4) with a space(to become G b4 or Gb 4)
  * YOU SHOULD ONLY SPECIFY A NAME IF ITS AN ALTERATION. CHORDS LIKE C9 MAY NOT WORK. THATS WHAT
  * ESSENTIAL TONES ARE FOR. Put a space after the root of the chord just to be safe.
  * Should have the order root, major/min/dim indication, 7, and then any order of other chord tones.
  * Only the root is necessary
  * @todo: error handling, research more ways to express chords
  */

/// @todo documentation, implementations, error handling for illegal inputs?
/// @todo change some public to private
class Chord
{
public:
    /// @brief the default constructor. Defaults to CMaj7 with 1357 octave 4 voicing
    /// Default duration is 0, default velocity is 0
    Chord();

    /// @overload Chord(std::string name)
    Chord(std::string name);

    /// @overload Chord(std::string name, vector<int> voicing)
    Chord(std::string name, std::vector<int> voicing);
    /*
    void setVoicing(vector<int> voicing);
    vector<int> voicing();
    std::vector<int> closestVoicing(Chord chord);
    void setDuration();
    void setVelocity();
    void setOctave(int octave);
    std::string name();
    std::set<int> voicing();
    */

    /// @brief Each of the following functions returns a note object corresponding to its degree
    Note first();
    Note second();
    Note third();
    Note fourth();
    Note fifth();
    Note sixth();
    Note seventh();

    /// @brief for user convenience, we decide to make first, second, fourth, and sixth
    /// also accessible as root, ninth, eleventh, and thirteenth respectively
    Note root();
    Note ninth();
    Note eleventh();
    Note thirteenth();
private:
    void _setFirst();
    void _setBottom();
    void _setSecond();
    void _setThird();
    void _setFourth();
    void _setFifth();
    void _setSixth();
    void _setSeventh();
    void _setTones();
    int _duration, _velocity, _octave;
    std::string _name;
    std::set<int> _essentialTones, _illegalTones;
    std::vector<int> _voicing;
    Note _bottom, _first, _second, _third, _fourth, _fifth, _sixth, _seventh;
    std::vector<Note*> notes = {&_first, &_second, &_third, &_fourth, &_fifth, &_sixth, &_seventh};
};

#endif // CHORD_H
