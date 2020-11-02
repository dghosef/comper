#include "note.h"
#include "note_numbers.h"
#include <regex>
#include <ctype.h>

using namespace std;

Note::Note() {
    _name = "C";
    _octave = MIDDLE_OCTAVE;
    _setNumber();
}

Note::Note(string name) {
    _name = name;
    _octave = MIDDLE_OCTAVE;
    _setNumber();
}

Note::Note(string name, int octave) {
    _name = name;
    _octave = octave;
    _setNumber();
}

Note::Note(int midiNumber) {
    _midiNumber = midiNumber;
    _setName();
    _setOctave();
}

Note & Note::operator=(string name) {
    setNote(name);
    return *this;
}

int Note::operator-(Note &note) {
    return this->distance(note);
}

bool Note::operator==(Note &note) {
    return note._midiNumber == this->_midiNumber;
}

bool Note::operator==(int midiNumber) {
    return midiNumber == _midiNumber;
}

bool Note::operator==(string name) {
    return getNumber(name) == _midiNumber;
}

bool Note::operator!=(Note &note) {
    return note._midiNumber != this->_midiNumber;
}

bool Note::operator!=(int midiNumber) {
    return midiNumber != _midiNumber;
}

bool Note::operator!=(string name) {
    return getNumber(name) != _midiNumber;
}

int Note::getNumber() {
    return _midiNumber;
}

int Note::getNumber(string name) {
    int midiNumber = MIDI_NUMBERS.at(toupper(name[0]));
    // Make sure that we don't count the note 'b' as 'bb'
    name = name.substr(1);
    // adjust for sharps and flats
    if(regex_search(name, regex("##")))
        midiNumber += 2;
    else if(regex_search(name, regex("#")))
        midiNumber++;
    else if(regex_search(name, regex("bb")))
        midiNumber -= 2;
    else if(regex_search(name, regex("b")))
        midiNumber--;
    if(midiNumber < 0) {
        midiNumber += NUM_NOTES;
    }
    midiNumber += NUM_NOTES * (_octave - MIDI_START_OCTAVE);
    return midiNumber;
}

int Note::getOctave() {
    return _octave;
}

string Note::getName() {
    return _name;
}

void Note::setNote(string name) {
    _name = name;
    _setNumber();
}

void Note::setOctave(int octave) {
    _octave = octave;
    _setNumber();
}

int Note::distance(Note &note) {
    return abs(note.getNumber() - this->getNumber());
}

int Note::shortestDistance(Note &note) {
    int longDistance = distance(note);
    if(longDistance > NUM_NOTES) {
        return min(12 - longDistance % NUM_NOTES, longDistance % NUM_NOTES);
    }
    return longDistance;
}

void Note::_setNumber() {
    _midiNumber = getNumber(_name);
}

void Note::_setName() {
    // Referenced the following because I'm a noob at c++. Idk if references like this are necessary
    // https://stackoverflow.com/questions/3136520/determine-if-map-contains-a-value-for-a-key
     auto it = MIDI_NAMES.find(_midiNumber % 12);
    if(it != MIDI_NAMES.end()) {
        _name = MIDI_NAMES.at(_midiNumber % 12);
    } else {
        // C# and Db are expressed as Db.
        _name = MIDI_NAMES.at(_midiNumber % 12 + 1);
        _name += "b";
    }
}

void Note::_setOctave() {
    _octave = MIDI_START_OCTAVE + _midiNumber / NUM_NOTES;
}

Note Note::closest(std::string name) {
    Note note(name, _octave);
    if(note.getNumber() < this->_midiNumber - NUM_NOTES / 2) {
        note.setOctave(note.getOctave() + 1);
    } else if(note.getNumber() > this->_midiNumber + NUM_NOTES / 2) {
        note.setOctave(note.getOctave() - 1);
    }
    return note;
}
