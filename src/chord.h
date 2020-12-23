/*
This file is part of Comper.

Comper is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Comper is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Comper.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CHORD_H
#define CHORD_H

#include <string>
#include <vector>
#include <array>
#include <algorithm> // std::copy_if
#include <stdexcept> // std::runtime_error

#include "note.h"

/**
  * @class Chord
  * @brief stores a chord's notes, duration, octave, velocity, voicing, and name
  * @author Joseph Tan
  *
  * The name of the chord should start with: root/bass(ex: C#/G) or just root(ex: Fb).
  * Everything should be lowercase except note names. Ex: F/B is valid but F/B Min is not.
  * By default, each of its tones are initialized to their corresponding degree on major scale
  * You can change this with alterations. Only specify a tone if you are altering it. For example,
  * C9 is not a valid chord name because you are not altering the 9. It should just be C.
  * You can have spaces separate the alterations. Ex: c# b4 b11. If you alter the same tone twice
  * it takes the sharp one. Ex: Bm b4#4 = Bm #4. You must put spaces if it's not possible to tell
  * your intention. Ex: C#9 should be changed to C# 9, C #9, or C# #9.
  *
  * After the root/bass, you should specify any alterations to the third if there are any.
  * For a b3, you can add m, -, ~, dim, hdim(half diminished), b3, min, or start with a lowercase
  * letter. Ex Cm = C~. For a #3, you can add #3. Note that dim only alters the 3rd and 7th but
  * not any other tones
  *
  * After that, in any order, you can add alterations for the other chord tones. Ex: C#dimb5#13
  * The alterations on the ninth can be b9, b2, #9, or #2
  * The alterations on the fourth can be b4, b11, #4, #11
  * The alterations on the fifth can be b5, dim, hdim, +, aug, #5
  * The alterations on the sixth can be b6, b13, #6, #13
  * The seventh will be a major 7 if 'maj' appears anywhere in the chord or if the chord only has
  * a bass/root in uppercase. Ex: C#, C#majb4b5, C#maj7, and C#minmaj all have major 7ths.
  * The seventh will be a diminished seventh if you alter the third to be 'dim'. A 'dim' overwrites
  * any other indications for the seventh
  * Otherwise, it will be dominant. You may also place a 'b7' to make it dominant and overwrite any
  * 'maj'. Note that 'dim' still has precedence. Ex: f#, C13, and G# b7 b9 are all dominant.
  * By default velocity and duration are 0 and need to manually be set
  */

class Chord
{
public:
    /// Sets chord to Cmaj7 octave 4 and 1357 voicing
    Chord();

    /// Sets chord name to `name` octave 4 and 1357 voicing
    Chord(const std::string name);

    /// Sets chord name to `name` octave to `octave` and 1357 voicing
    Chord(const std::string name, const int octave);

    /// Sets chord name to `name` octave to `octave` voicing to `voicing`
    Chord(const std::string name, const int octave, const std::vector<int> voicing);

    /// A copy constructor is neccessary because we have a const member(_notes)
    Chord(const Chord &chord);

    /// Copy assignment is neccessary because we have a const member(_notes)
    Chord &operator=(const Chord &chord);

    /// Sets our chord name to `name` and adjusts notes and voicing accordingly.
    Chord &operator=(const std::string name);

    /// Compares our chord to `chord`
    bool operator==(const Chord &chord) const;

    /// Compares our chord to `chord`. Functionally equivalent to !(*this == chord)
    bool operator!=(const Chord &chord) const;

    /// Set this chord's members equal to those of `chord`
    void setEqual(const Chord &chord);

    /// Set our note's duration to `duration`
    void setDuration(const int duration);

    /// Set our note's velocity to `velocity`
    void setVelocity(const int velocity);

    /// Sets octave of our chord's bass note to `octave`. Rebuilds the chord if necessary
    void setOctave(const int octave);

    /// Sets our chord name to `name` and adjusts notes and voicing accordingly.
    void setName(const std::string name);

    /**
     * Sets the voicing of the chord
     * @param voicingNumbers a vector containing the degrees in our voicing
     * @example A Cmaj7 chord with {1, 3, 5, 7, 9, 9} would have c, e, g, b, d, d(an octave higher)
     */
    void setVoicing(const std::vector<int> voicingNumbers);

    /**
     * Given a vector of possible voicings expressed with ints and a predicate
     * pick the voicing that the predicate returns the lowest score on. If the predicate
     * returns the same score for multiple voicings, returns the one that occured earlier in the vector
     * @param voicingNumbers a vector of possible voicings
     * @param pred a function that takes a voicing and a chord and returns an integer
     *   pred must have the form int pred(Chord chord, vector<int> voicing). 
     *   *this will be passed as `chord` and each voicing will be passed as `voicing`.
     */
    template<typename Func>
    void setVoicing(const std::vector<std::vector<int>> voicings, const Func pred) {
        std::vector<std::vector<int>> acceptableVoicings;
        /* Check the predicate on each element of voicings and if the predicate returns true, copy
           to acceptableVoicings */
        std::copy_if(voicings.begin(), voicings.end(), back_inserter(acceptableVoicings),
                     [pred, this](const std::vector<int> voicing){return pred(*this, voicing);});
        if(acceptableVoicings.empty()) {
            throw std::runtime_error("pred returns false on all voicings");
        }
        /* Pick a random element from our acceptable voicings and set our voicing to it.
        because voicings typically contain < 10 notes, random bias shouldn't be a problem
        @cite https://stackoverflow.com/questions/6942273/how-to-get-a-random-element-from-a-c-container */
        srand((unsigned)time(NULL));
        auto randIt = acceptableVoicings.begin();
        std::advance(randIt, std::rand() % acceptableVoicings.size());
        setVoicing(*std::min_element(voicings.begin(), voicings.end(), 
                    [&pred, this] (std::vector<int> voicing1, std::vector<int> voicing2)
                    {return pred(*this, voicing1) < pred(*this, voicing2);}));
    }

    /// Return the duration of our note
    int duration() const;

    /// Return the velocity of our note
    int velocity() const;

    /// Return the octave our note
    int octave() const;

    /// Return the name of our note
    std::string name() const;

    /// Return a vector of ints that represents the degrees in our voicing
    std::vector<int> voicingNumbers() const;

    /// Return a vector of notes that represent the voicing of our chord
    std::vector<Note> voicing() const;

    /**
     * Return a vector of notes representing all our notes in order.
     * Note[0] is the bass note, Note[1] is the root, Note[2] is the ninth, etc
     */
    std::vector<Note> notes() const;

    /// Returns the bass note of the chord. Only different than first() with slash chords
    Note bass() const;

    /// Returns the first degree of the chord
    Note first() const;

    /// Returns the second degree of the chord
    Note second() const;

    /// Returns the third degree of the chord
    Note third() const;

    /// Returns the fourth degree of the chord
    Note fourth() const;

    /// Returns the fifth degree of the chord
    Note fifth() const;

    /// Returns the sixth degree of the chord
    Note sixth() const;

    /// Returns the seventh degree of the chord
    Note seventh() const;

    /// Returns the root. Equivalent to first()
    Note root() const;

    /// Returns the ninth of the chord. Equivalent to second()
    Note ninth() const;

    /// Returns the eleventh of the chord. Equivalent to fourth()
    Note eleventh() const;

    /// Returns the thirteenth of the chord. Equivalent to sixth()
    Note thirteenth() const;

private:
    // Sets our notes according to the _name. _setBass should be called then _setFirst then the rest
    void _setBass();
    void _setFirst();
    void _setSecond();
    void _setThird();
    void _setFourth();
    void _setFifth();
    void _setSixth();
    void _setSeventh();

    // Calls the above 7 functions in order
    void _setTones();

    // Sets our voicing according to _voicingNumbers
    void _setVoicing();

    // Raises a runtime_exception if our chord name breaks the specification above
    void _checkName() const;

    /*
     * Each note will have a duration and velocity value of _duration and _velocity respectively.
     * _bass will have the octave _octave and the rest build on top of it
     */
    int _duration, _velocity, _octave;

    // The name of our chord. Must be in the form described above
    std::string _name;

    // A series of chord tones indicating the voicing of this chord that will be played.
    std::vector<int> _voicingNumbers;

    // The notes that comprise our voicing
    std::vector<Note> _voicing;

    // Note objects representing our chord tones
    Note _bass, _first, _second, _third, _fourth, _fifth, _sixth, _seventh;

    // Allows for easy access of, iteration through, and indexing of the notes of our chord
    const std::array<Note*, 8> _notes = {&_bass, &_first, &_second, &_third, &_fourth, &_fifth,
                                       &_sixth, &_seventh};
};

#endif // CHORD_H
