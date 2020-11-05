#include "note.h"
#include "note_numbers.h"
#include <regex>
#include <ctype.h>

using namespace std;

/// @todo Make sure we use const and & at the right place in arguments and return types
Note::Note() {
    _name = "C";
    _octave = MIDDLE_OCTAVE;
    _setNumber();
    _duration = 0;
    _velocity = 0;
}

Note::Note(string name) {
    _name = name;
    _octave = MIDDLE_OCTAVE;
    _setNumber();
    _duration = 0;
    _velocity = 0;
}

Note::Note(string name, int octave) {
    _name = name;
    _octave = octave;
    _setNumber();
    _duration = 0;
    _velocity = 0;
}

Note::Note(string name, int octave, int duration) {
    _name = name;
    _octave = octave;
    _setNumber();
    _duration = duration;
    _velocity = 0;
}

Note::Note(string name, int octave, int duration, int velocity) {
    _name = name;
    _octave = octave;
    _setNumber();
    _duration = duration;
    _velocity = velocity;
}

Note::Note(int midiNumber) {
    _midiNumber = midiNumber;
    _setName();
    _setOctave();
    _duration = 0;
    _velocity = 0;
}

Note::Note(int midiNumber, int duration) {
    _midiNumber = midiNumber;
    _setName();
    _setOctave();
    _duration = duration;
    _velocity = 0;
}

Note::Note(int midiNumber, int duration, int velocity) {
    _midiNumber = midiNumber;
    _setName();
    _setOctave();
    _duration = duration;
    _velocity = velocity;
}

Note & Note::operator=(string name) {
    setNote(name);
    return *this;
}

Note & Note::operator=(int number) {
    _midiNumber = number;
    _setName();
    _setOctave();
    return *this;
}

int Note::operator-(Note &note) {
    return this->distance(note);
}

Note Note::operator+(int amount) {
    return Note(_midiNumber + amount);
}

Note & Note::operator+=(int amount) {
    setNumber(_midiNumber + amount);
    return *this;
}

bool Note::operator==(const Note &note) {
    return note._midiNumber == this->_midiNumber;
}

bool Note::operator==(int midiNumber) {
    return midiNumber == _midiNumber;
}

bool Note::operator==(string name) {
    return number(name) == _midiNumber;
}

bool Note::operator!=(Note &note) {
    return note._midiNumber != this->_midiNumber;
}

bool Note::operator!=(int midiNumber) {
    return midiNumber != _midiNumber;
}

bool Note::operator!=(string name) {
    return number(name) != _midiNumber;
}

bool Note::operator>(Note &note) {
    return this->number() > note.number();
}

bool Note::operator<(Note &note) {
    return this->number() < note.number();
}

int Note::number() {
    return _midiNumber;
}

int Note::number(string name) {
    int midiNumber = MIDI_NUMBERS.at(toupper(name[0]));
    // Make sure that we don't count the note 'b' as 'bb'
    name = name.substr(1);
    // adjust for sharps and flats
    if(regex_search(name, regex("##"))) {
        midiNumber += 2;
    } else if(regex_search(name, regex("#"))) {
        midiNumber++;
    } else if(regex_search(name, regex("bb"))) {
        midiNumber -= 2;
    } else if(regex_search(name, regex("b"))) {
        midiNumber--;
    }
    if(midiNumber < 0) {
        midiNumber += NUM_NOTES;
    }
    midiNumber += NUM_NOTES * (_octave - MIDI_START_OCTAVE);
    return midiNumber;
}

int Note::octave() {
    return _octave;
}

int Note::duration() {
    return _duration;
}

int Note::velocity() {
    return _velocity;
}

string Note::name() {
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

void Note::setNumber(int number) {
    _midiNumber = number;
    _setName();
    _setOctave();
}

void Note::setVelocity(int velocity) {
    _velocity = velocity;
}

void Note::setDuration(int duration) {
    _duration = duration;
}

int Note::distance(Note &note) {
    return abs(note.number() - this->number());
}

int Note::shortestDistance(Note &note) {
    int longDistance = distance(note);
    if(longDistance > NUM_NOTES) {
        return min(NUM_NOTES - longDistance % NUM_NOTES, longDistance % NUM_NOTES);
    }
    return longDistance;
}

void Note::_setNumber() {
    _midiNumber = number(_name);
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
    if(note.number() < this->_midiNumber - NUM_NOTES / 2) {
        note.setOctave(note.octave() + 1);
    } else if(note.number() > this->_midiNumber + NUM_NOTES / 2) {
        note.setOctave(note.octave() - 1);
    }
    return note;
}

