#ifndef NOTE_H
#define NOTE_H
#include <string>

/**
 * @class Note
 * @brief Stores and calculates a note's name, midi number, duration, velocity, and octave
 * @author Joseph Tan
 * @details B# Octave 3 is the same as C Octave 4. getOctave will always return the octave with our
 * note name in mind rather than computing an octave purely on the midi number. Name is always
 * user defined, the default or altered by setting the note's midi number in which it defaults to
 * a natural note if possible otherwise a flat note.
 * @example note.setNumber(60) == "C"; note.setNumber(61) == "Db";
 * @todo rearrange private and public a wee bit
 * @todo error handling for illegal inputs?
 * @todo better documentation/learn doxygen fully
 * @todo rename some of the get functions just to their name
 */

class Note {

public:
    /// @brief Default our note to middle C
    Note();

    /// @overload Note(std::string name)
    /// @brief defaults to same octave as middle C with duration 0 and velocity 0
    Note(std::string name);

    /// @overload Note(std::string name, int octave)
    Note(std::string name, int octave);

    /// @overload Note(std::string name, int octave, int duration)
    Note(std::string name, int octave, int duration);

    /// @overload Note(std::string name, int octave, int duration, int velocity)
    Note(std::string name, int octave, int duration, int velocity);

    /// @overload Note(int midiNumber)
    /// @brief constructs note given a midi number. By default uses flats.
    Note(int midiNumber);

    /// @overload Note(int midiNumber)
    /// @brief constructs note given a midi number. By default uses flats.
    Note(int midiNumber, int duration);

    /// @overload Note(int midiNumber)
    /// @brief constructs note given a midi number. By default uses flats.
    Note(int midiNumber, int duration, int velocity);

    /// @brief change note name. Automatically changes midi number along with it. Keeps octave
    /// @param name is the name of the new note
    Note & operator=(std::string name);

    /// @brief change note number. Automatically changes midi name and octave along with it.
    /// @param name is the name of the new note
    Note & operator=(int number);

    /// @brief same as the distance function
    /// @param &note the note that will be subtracted
    int operator-(Note &note);

    /// @brief returns a note that is our note incremented amount semitones
    /// @param amount the amount to increment the note by
    Note operator+(int amount);

    /// @brief Assigns our note to equal note + amount
    /// @param amount the amount to increment the note by
    Note & operator+=(int amount);

    /// @brief checks if &note has the same number
    /// @param note is the note being compared
    bool operator==(Note const &note);

    /// @overload operator==(int midiNumber)
    bool operator==(int midiNumber);

    /// @overload operator==(int name)
    /// checks if the current name is a valid way to express the note's midi number
    bool operator==(std::string name);

    /// @brief checks if &note has the same number
    /// @param note is the note being compared
    bool operator!=(Note &note);

    /// @overload operator==(int midiNumber)
    bool operator!=(int midiNumber);

    /// @overload operator==(int name)
    /// checks if the current name is a valid way to express the note's midi number
    bool operator!=(std::string name);

    /// @brief checks if &note has a larger number
    /// @param note is the note being compared
    bool operator>(Note &note);

    /// @brief checks if &note has a smaller number
    /// @param note is the note being compared
    bool operator<(Note &note);

    /// @return the note number
    int number();

    /// @overload getNumber(std::string name)
    /// @brief note number of closest note with note name name.
    int number(std::string name);

    /// @return the note octave
    int octave();

    /// @return the duration of the note
    int duration();

    /// @return the velocity of the note
    int velocity();

    /// @return the note name
    std::string name();

    /// @brief Changes the note
    /// @param name a string specifiying the name of the new note. In same octave as current note.
    void setNote(std::string name);

    /// @brief Changes the octave
    /// @param name a octave specifiying the name of the new octave
    void setOctave(int octave);

    /// @brief sets the note number and note name/octave accordingly. Defaults to flats
    void setNumber(int number);

    /// @brief alters the velocity of the note
    void setVelocity(int velocity);

    /// @brief alters the duration of the note
    void setDuration(int duration);

    /// @returns Returns the distance in semitones between 2 notes
    /// @param Note - the note we are comparing too
    int distance(Note &note);

    /// @returns returns distance from the closest note with the same name as note
    /// @param note - the note we are comparing too
    int shortestDistance(Note &note);

    /// @returns a Note object that represents the nearest note with name to the class's note
    Note closest(std::string name);


protected:
    std::string _name;
    int _midiNumber;
    int _octave;
    int _duration;
    int _velocity;

    // Adjusts class number, name, and octave depending on _name, _midiNumber, and _octave
    void _setNumber();
    void _setName();
    void _setOctave();
};
#endif // NOTE_H

