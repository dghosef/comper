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

#include <regex>
#include <ctype.h>

#include "note.h"
#include "note_numbers.h"

using namespace std;

Note::Note() {
    _octave = MIDDLE_OCTAVE;
    _duration = 0;
    _velocity = 0;
    setName("C");
}

Note::Note(const string name) {
    _octave = MIDDLE_OCTAVE;
    _duration = 0;
    _velocity = 0;
    setName(name);
}

Note::Note(const string name, const int octave) {
    _octave = octave;
    _duration = 0;
    _velocity = 0;
    setName(name);
}

Note::Note(const string name, const int octave, const int duration) {
    _octave = octave;
    _duration = duration;
    _velocity = 0;
    setName(name);
}

Note::Note(const string name, const int octave, const int duration, const int velocity) {
    _octave = octave;
    _duration = duration;
    _velocity = velocity;
    setName(name);
}

Note::Note(const int midiNumber) {
    _duration = 0;
    _velocity = 0;
    _midiNumber = midiNumber;
    _setName();
    _setOctave();
}

Note::Note(const int midiNumber, const int duration) {
    _duration = duration;
    _velocity = 0;
    _midiNumber = midiNumber;
    _setName();
    _setOctave();
}

Note::Note(const int midiNumber, const int duration, const int velocity) {
    _duration = duration;
    _velocity = velocity;
    _midiNumber = midiNumber;
    _setName();
    _setOctave();
}

Note &Note::operator=(const string name) {
    setName(name);
    return *this;
}

Note &Note::operator=(const int number) {
    _midiNumber = number;
    _setName();
    _setOctave();
    return *this;
}

Note Note::operator+(const int amount) const {
    return Note(_midiNumber + amount);
}

Note &Note::operator+=(const int amount) {
    setNumber(_midiNumber + amount);
    return *this;
}

int Note::operator-(const Note &note) const {
    return this->distance(note);
}

bool Note::operator==(const Note &note) const {
    return note._midiNumber == this->_midiNumber &&
            note._duration == this->_duration &&
            note._velocity == this->_velocity;
}

bool Note::operator==(const int midiNumber) const {
    return midiNumber == _midiNumber;
}

bool Note::operator==(const string name) const {
    return number(name) == _midiNumber;
}

bool Note::operator!=(const Note &note) const {
    return !(*this == note);
}

bool Note::operator!=(const int midiNumber) const {
    return !(*this == midiNumber);
}

bool Note::operator!=(const string name) const {
    return !(*this == name);
}

bool Note::operator>(const Note &note) const {
    return this->number() > note.number();
}

bool Note::operator<(const Note &note) const {
    return this->number() < note.number();
}

bool Note::operator>=(const Note &note) const {
    return this->number() >= note.number();
}

bool Note::operator<=(const Note &note) const {
    return this->number() <= note.number();
}

string Note::name() const {
    return _name;
}

int Note::number(string name) const {
    int midiNumber = MIDI_NUMBERS.at(toupper(name[0]));
    // Make sure that we don't count the note 'b' as 'bb'
    name = name.substr(1);
    // Adjust for sharps and flats
    if(regex_search(name, regex("##"))) {
        midiNumber += 2;
    } else if(regex_search(name, regex("#"))) {
        ++midiNumber;
    } else if(regex_search(name, regex("bb"))) {
        midiNumber -= 2;
    } else if(regex_search(name, regex("b"))) {
        --midiNumber;
    }
    if(midiNumber < 0) {
        midiNumber += NUM_NOTES;
    }
    midiNumber += NUM_NOTES * (_octave - MIDI_START_OCTAVE);
    return midiNumber;
}

int Note::number() const {
    return _midiNumber;
}

int Note::octave() const {
    return _octave;
}

int Note::duration() const {
    return _duration;
}

int Note::velocity() const {
    return _velocity;
}

void Note::setName(const string name) {
    _name = name;
    _checkName();
    _setNumber();
}

void Note::setOctave(const int octave) {
    _octave = octave;
    _setNumber();
}

void Note::setNumber(const int number) {
    _midiNumber = number;
    _setName();
    _setOctave();
}

void Note::setVelocity(const int velocity) {
    _velocity = velocity;
}

void Note::setDuration(const int duration) {
    _duration = duration;
}

int Note::distance(const Note &note) const {
    return abs(note.number() - this->number());
}

int Note::shortestDistance(const Note &note) const {
    int longDistance = distance(note);
    if(longDistance > NUM_NOTES) {
        return min(NUM_NOTES - longDistance % NUM_NOTES, longDistance % NUM_NOTES);
    }
    return longDistance;
}

Note Note::closest(const std::string name) const {
    Note note(name, _octave);
    if(note.number() < this->_midiNumber - NUM_NOTES / 2) {
        note.setOctave(note.octave() + 1);
    } else if(note.number() > this->_midiNumber + NUM_NOTES / 2) {
        note.setOctave(note.octave() - 1);
    }
    return note;
}

void Note::_setNumber() {
    _midiNumber = number(_name);
}

void Note::_setName() {
    /// @cite https://stackoverflow.com/questions/3136520/determine-if-map-contains-a-value-for-a-key
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

void Note::_checkName() {
    /* A valid note name must start with a valid letter. It doesn't really matter if
       it has a letter and then garbage after(like C#KJHDF would just be interpreted as C#) */
    regex validNameExp("^[A-Ga-g](.*)");
    if(!regex_match(_name, validNameExp)) {
        throw runtime_error(_name + " is an invalid note name");
    }
}
