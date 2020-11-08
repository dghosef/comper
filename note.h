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

#ifndef NOTE_H
#define NOTE_H

#include <string>

/**
 * @class Note
 * @brief Stores and calculates a note's name, midi number, duration, velocity, and octave
 * name is standard naming for notes(C#, F, Db, E##, Gbb, etc)
 * @note when the note generates a name from a note number, it defaults to flats for accidentals
 * midi number is the number that corresponds to the note in midi files
 * duration is in the form (notes)/whole note.
 * @example if duration = 4, duration is (4 notes)/(whole note) or a quarter note
 * velocity is the velocity as specified in the midi file
 * octave is the octave number. Middle C on the piano is octave 5. Note that B# oct. 6 == C oct. 7
 * @author Joseph Tan
 */

class Note {

public:
    /// Default our note to middle C duration to 0 velocity to 0
    Note();

    /// Set our note name to `name` with octave 5 duration to 0 velocity to 0
    Note(const std::string name);

    /// Set our note name to `name` with octave `octave` duration to 0 velocity to 0
    Note(const std::string name, const int octave);

    /// Set our note name to `name` with octave `octave` duration to `duration` velocity to 0
    Note(const std::string name, const int octave, const int duration);

    /// Set note name to `name` with octave `octave` duration to `duration` velocity to `velocity`
    Note(const std::string name, const int octave, const int duration, const int velocity);

    /// Given `midiNumber` sets octave and note name. Sets velocity to 0 and duration to 0
    Note(const int midiNumber);

    /// Given `midiNumber` sets octave and note name. Sets velocity to `velocity` and duration to 0
    Note(const int midiNumber, const int duration);

    /**
     * Given `midiNumber` sets octave and note name.
     * sets velocity to `velocity` and duration to `duration`
    */
    Note(const int midiNumber, const int duration, const int velocity);

    /// Changes our name to `name` and adjusts midiNumber accordingly
    Note &operator=(const std::string name);

    /// Changes our midi number to `number` and adjusts name and octave accordingly
    Note &operator=(const int number);

    /// Returns a note that is our note incremented by `amount` semitones
    Note operator+(const int amount) const;

    /// Increments our note by `amount` semitones
    Note &operator+=(const int amount);

    /// Returns the absolute distance between this note and `note`
    int operator-(const Note &note) const;
    
    /// Compares this note with `note`
    bool operator==(const Note& note) const;

    /// Compares this note's midi number with `midiNumber`
    bool operator==(const int midiNumber) const;

    /// Checks if `name` is a valid way to express our note
    bool operator==(const std::string name) const;

    /// Compares this note with `note`
    bool operator!=(const Note &note) const;

    /// Compares this note's midi number with `midiNumber`
    bool operator!=(const int midiNumber) const;

    /// Checks if `name` is a valid way to express our note
    bool operator!=(const std::string name) const;

    /// Compares this note with `note`
    bool operator>(const Note &note) const;

    /// Compares this note with `note`
    bool operator<(const Note &note) const;

    /// Compares this note with `note`
    bool operator>=(const Note &note) const;

    /// Compares this note with `note`
    bool operator<=(const Note &note) const;

    /// Return our note name
    std::string name() const;

    /// Given a name, return the midi number of a note with the same name in the same octave
    int number(std::string name) const;

    /// Return our midiNumber
    int number() const;

    /// Return our octave
    int octave() const;

    /// Return the duration of our note
    int duration() const;

    /// Return the velocity of our note
    int velocity() const;

    /// Keep octave the same just change the note name and midinumber according to `name`
    void setName(const std::string name);

    /// Set our octave to `octave` and adjust our midi number accordingly
    void setOctave(const int octave);

    /// Set our midi number to `number` and adjust name and octave accordingly
    void setNumber(const int number);

    /// Set our velocity to `velocity`
    void setVelocity(const int velocity);

    /// Set the note duration to `duration`
    void setDuration(const int duration);

    /// Return the distance between our note and `note` in semitones
    int distance(const Note &note) const;

    /// Return the distance between our note and the closest note with the same name as `note`
    int shortestDistance(const Note &note) const;

    /// Returns a Note object that represents the nearest note with name to the class's note
    Note closest(const std::string name) const;


protected:
    std::string _name;
    int _midiNumber, _octave, _velocity, _duration;

    // Adjusts class number, name, and octave depending on _name, _midiNumber, and _octave
    void _setNumber();
    void _setName();
    void _setOctave();

    // Throws an error if the note name is incorrect
    void _checkName();
};
#endif // NOTE_H

