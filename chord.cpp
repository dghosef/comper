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

#include <string>
#include <vector>
#include <regex>
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::equal

#include "chord.h"
#include "note.h"
#include "note_numbers.h"

Chord::Chord() {
    _duration = 0, _velocity = 0, _octave = 4;
    _voicingNumbers = {1, 3, 5, 7};
    setName("Cmaj7");
}

Chord::Chord(const std::string name) {
    _duration = 0, _velocity = 0, _octave = 4;
    _voicingNumbers = {1, 3, 5, 7};
    setName(name);
}

Chord::Chord(const std::string name, const int octave) {
    _duration = 0, _velocity = 0, _octave = octave;
    _voicingNumbers = {1, 3, 5, 7};
    setName(name);
}

Chord::Chord(const std::string name, const int octave, const std::vector<int> voicing) {
    _duration = 0, _velocity = 0, _octave = octave;
    _voicingNumbers = voicing;
    setName(name);
}

Chord::Chord(const Chord &chord) {
    setEqual(chord);
}

Chord &Chord::operator=(const Chord &chord) {
    setEqual(chord);
    return *this;
}

Chord &Chord::operator=(const std::string name) {
    setName(name);
    return *this;
}

/// @cite https://stackoverflow.com/questions/39855341/equals-operator-on-stl-std::vector-of-pointers
bool Chord::operator==(const Chord &chord) const {
    return chord._voicingNumbers == this->_voicingNumbers &&
        // Dereference each member of `this`._notes std::vector and `chord`._notes std::vector and compare
            std::equal(begin(this->_notes), end(this->_notes), begin(chord._notes),
                 [](const Note *a, const Note *b){return *a == *b;});
}

bool Chord::operator!=(const Chord &chord) const {
    return !(*this == chord);
}

void Chord::setEqual(const Chord &chord) {
    this->_name = chord._name;
    this->_octave = chord._octave;
    this->_duration = chord._duration;
    this->_velocity = chord._velocity;
    this->_voicing = chord._voicing;
    this->_voicingNumbers = chord._voicingNumbers;

    // Copy each of the notes from `chord` into this chord
    for(size_t i = 0; i < chord._notes.size(); i++) {
        *(this->_notes[i]) = *(chord._notes[i]);
    }
}

void Chord::setDuration(const int duration) {
    for(auto it = _notes.begin(); it < _notes.end(); ++it) {
        (*it)->setDuration(duration);
    }
    for(auto it = _voicing.begin(); it < _voicing.end(); ++it) {
        (it)->setDuration(duration);
    }
    _duration = duration;
}

void Chord::setVelocity(const int velocity) {
    for(auto it = _notes.begin(); it < _notes.end(); ++it) {
        (*it)->setVelocity(velocity);
    }
    for(auto it = _voicing.begin(); it < _voicing.end(); ++it) {
        (it)->setVelocity(velocity);
    }
    _velocity = velocity;
}

void Chord::setOctave(const int octave) {
    for(auto it = _notes.begin(); it < _notes.end(); ++it) {
        // Add the difference between the target octave and current octave to each note's octave
        (*it)->setOctave((*it)->octave() + (octave - _octave));
    }
    _octave = octave;
    _setVoicing();
}

void Chord::setName(const std::string name) {
    _name = name;
    _checkName();
    _setTones();
    _setVoicing();
}

void Chord::setVoicing(const std::vector<int> voicingNumbers) {
    _voicingNumbers = voicingNumbers;
    _setVoicing();
}

int Chord::duration() const {
    return _duration;
}

int Chord::velocity() const {
    return _velocity;
}

int Chord::octave() const {
    return _octave;
}

std::string Chord::name() const {
    return _name;
}

std::vector<int> Chord::voicingNumbers() const {
    return _voicingNumbers;
}

std::vector<Note> Chord::voicing() const {
    return _voicing;
}

std::vector<Note> Chord::notes() const {
    std::vector<Note> ret;
    for(Note *note : _notes) {
        ret.push_back(*note);
    }
    return ret;
}
Note Chord::bass() const {
    return _bass;
}

Note Chord::first() const {
    return _first;
}

Note Chord::second() const {
    return _second;
}

Note Chord::third() const {
    return _third;
}

Note Chord::fourth() const {
    return _fourth;
}

Note Chord::fifth() const {
    return _fifth;
}

Note Chord::sixth() const {
    return _sixth;
}

Note Chord::seventh() const {
    return _seventh;
}

Note Chord::root() const {
    return first();
}

Note Chord::ninth() const {
    return second();
}

Note Chord::eleventh() const {
    return fourth();
}

Note Chord::thirteenth() const {
    return sixth();
}

void Chord::_setBass() {
    _bass.setOctave(_octave);
    _bass.setDuration(_duration);
    _bass.setVelocity(_velocity);
    std::regex _bassRegex = std::regex("(^[A-Ga-g](#|b)?)|(/[A-Ga-g](#|b)?)");
    std::sregex_iterator end;
    // Augmented and diminished messages are falsely recognized as 'a' and 'd' notes so erase them
    std::string nameCleaned = std::regex_replace(_name,
                                       std::regex("((aug)|(Aug)|(dim)|(Dim)|(hdim)|(Hdim)).*"), "");
    /* Read through all possible bass notes and pick the last one.
       Ex: The loop for C#/Gb would look like C -> C# -> G -> Gb. Then we pick the Gb */
    for(std::sregex_iterator it(nameCleaned.begin(), nameCleaned.end(), _bassRegex);
        it != end; ++it) {
        // If our bass note is after a slash, remove the slash from the note
        _bass = (it->str())[0] == '/' ? (it->str()).substr(1) : it->str();
    }
}

void Chord::_setFirst() {
    _first = Note(_name.substr(0, 2), _octave, _duration, _velocity);
    if(_first < _bass) {
        _first.setOctave(_bass.octave() + 1);
    }
}

void Chord::_setSecond() {
    int _secondDist = std::regex_search(_name, std::regex("(b9)|(b2)")) ? 1 : 2;
    _secondDist = std::regex_search(_name, std::regex("(#9)|(#2)")) ? 3 : _secondDist;
    _second = _first + _secondDist;
}

void Chord::_setThird() {
    int _thirdDist = std::regex_search(_name, std::regex("-|~|(^[a-gA-G](#|b)?( )?(/.*)?m(#|b| |[1-9]|(aug)|"
                                               "\\+)|m$)|^[a-g]|(b3)|(min)|(dim)|(hdim)")) ? 3 : 4;
    _thirdDist = std::regex_search(_name, std::regex("#3")) ? 5 : _thirdDist;
    _third = _first + _thirdDist;
}

void Chord::_setFourth() {
    int _fourthDist = std::regex_search(_name, std::regex("(b4)|(b11)")) ? 4 : 5;
    _fourthDist = std::regex_search(_name, std::regex("(#11)|(#4)")) ? 6 : _fourthDist;
    _fourth = _first + _fourthDist;
}

void Chord::_setFifth() {
    int _fifthDist = std::regex_search(_name, std::regex("(b5)|(dim)|(hdim)")) ? 6 : 7;
    _fifthDist = std::regex_search(_name, std::regex("(#5)|\\+|(aug)")) ? 8 : _fifthDist;
    _fifth = _first + _fifthDist;
}

void Chord::_setSixth() {
    int _sixthDist = std::regex_search(_name, std::regex("(b6)|(b13)")) ? 8 : 9;
    _sixthDist = std::regex_search(_name, std::regex("(#6)|(#13)")) ? 10 : _sixthDist;
    _sixth = _first + _sixthDist;
}

void Chord::_setSeventh() {
    int _seventhDist = std::regex_search(_name, std::regex("maj|[A-G]#?b?$")) ? 11 : 10;
    _seventhDist = std::regex_search(_name, std::regex("b7")) ? 10 : _seventhDist;
    _seventhDist = std::regex_search(_name, std::regex("[a-gA-G](#|b)?( )?(dim)")) ? 9 : _seventhDist;
    _seventh = _first + _seventhDist;
}

void Chord::_setTones() {
    _setBass();
    _setFirst();
    _setSecond();
    _setThird();
    _setFourth();
    _setFifth();
    _setSixth();
    _setSeventh();
}

void Chord::_setVoicing() {
    _voicing = {*(_notes[_voicingNumbers[0]])};
    for(auto it = _voicingNumbers.begin() + 1; it < _voicingNumbers.end(); ++it) {
        int index = *it > NUM_DEGREES ? *it % NUM_DEGREES : *it; // Convert 9 to 2, 11 to 4, etc
        int octave = _voicing.back().octave();
        _voicing.push_back(*_notes[index]);
        _voicing.back().setOctave(octave);
        // If our note is below the previous note in the voicing, bump its octave up
        if(_voicing.back() <= *(_voicing.rbegin() + 1)) {
            _voicing.back().setOctave(_voicing.back().octave() + 1);
        }
    }
}

void Chord::_checkName() const {
    // Make sure we have nothing like C#9(is it C# 9, C #9, or C# #9?)
    if(std::regex_match(_name, std::regex("[A-Ga-g](#|b)[1-9]"))) {
        throw std::runtime_error(_name + " contains unclear placement of initial accidental");
    }
    // Regex expressions to check each degree
    std::string validNote("^([a-gA-G](#?|b?)(\\/[a-gA-G](#?b?))?)");
    std::string validNinth("((b9)|(#9)|(b2)|(#2))");
    std::string validThird("((m)|(-)|(~)|(dim)|(hdim)|(b3)|(#3)|(min))");
    std::string validFourth("((b4)|(#4)|(b11)|(#11))");
    std::string validFifth("((#5)|(\\+)|(b5)|(aug))");
    std::string validSixth("((b13)|(#13)|(b6)|(#6))");
    std::string validSeventh("((b7)|(7)|(maj)|(maj7))");
    std::string space("( )");
    std::string regexp = ("( )*") + validThird + "?";
    std::vector<std::string> regexes = {validNinth, validFourth, space,
                              validFifth, validSixth, validSeventh};
    // Check one degree at a time
    for(auto it = regexes.begin(); it < regexes.end(); ++it) {
        regexp.append("|" + *it);
    }
    // Apart from the root, bass, and third, the degrees can appear in any order and any quantity
    regexp = validNote + "(" + regexp + ")*";
    if(!std::regex_match(_name, std::regex(regexp))) {
        throw std::runtime_error(_name + " is an invalid name");
    }
}
