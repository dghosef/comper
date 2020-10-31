#ifndef NOTE_H
#define NOTE_H
#include <string>

/**
 * @class Note
 * @brief Stores and calculates a note's name, midi number, and octave
 * @author Joseph Tan
 *
 * @details B# Octave 3 is the same as C Octave 4.
 * @todo implement closest note
 * @todo implement time?
 */

class Note {

public:
    /// @brief Default our note to middle C
    Note();

    /// @overload Note(std::string name)
    /// @brief defaults to same octave as middle C
    Note(std::string name);

    /// @overload Note(std::string name, int octave)
    Note(std::string name, int octave);

    /// @overload Note(int midiNumber)
    /// @brief constructs note given a midi number. By default uses flats.
    Note(int midiNumber);

    /// @brief copy constructor
    /// @param &note - the note being copied
    Note(const Note &note);

    /// @brief set note equal to another note
    /// @param &note the note that will be copied
    Note & operator=(const Note &note);

    /// @brief change note name. Automatically changes midi number along with it. Keeps octave
    /// @param name is the name of the new note
    Note & operator=(std::string name);

    /// @brief same as the distance function
    /// @param &note the note that will be subtracted
    int operator-(Note &note);

    /// @brief checks if &note has the same number
    /// @param note is the note being compared
    bool operator==(Note &note);

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

    /// @return the note number
    int getNumber();

    /// @overload getNumber(std::string name)
    int getNumber(std::string name);

    /// @return the note octave
    int getOctave();

    /// @return the note name
    std::string getName();

    /// @brief Changes the note
    /// @param name a string specifiying the name of the new note
    void setNote(std::string name);

    /// @brief Changes the octave
    /// @param name a octave specifiying the name of the new octave
    void setOctave(int octave);

    /// @returns Returns the distance in semitones between 2 notes
    /// @param Note - the note we are comparing too
    int distance(Note &note);

    /// @returns returns distance from the closest note with the same name as note
    /// @param note - the note we are comparing too
    int shortestDistance(Note &note);

    /// @returns a Note object that represents the nearest note with name to the class's note
    Note closest(std::string name);


private:
    std::string _name;
    int _midiNumber;
    int _octave;

    // Adjusts class number, name, and octave depending on _name, _midiNumber, and _octave
    void _setNumber();
    void _setName();
    void _setOctave();
};

#endif // NOTE_H
