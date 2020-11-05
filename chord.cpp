#include "chord.h"
#include "note.h"
#include <string>
#include <set>
#include <regex>
using namespace std;
/// @note when implementing error checks, watch for too many digits
// referenced: https://www.regular-expressions.info/stdregex.html for c++ regex
Chord::Chord() {
    _duration = 0, _velocity = 0, _octave = 4;
    _voicing = {1, 3, 5, 7};
    _name = "CMaj7";
    _setTones();
}

Chord::Chord(string name) {
    _duration = 0, _velocity = 0, _octave = 4;
    _voicing = {1, 3, 5, 7};
    _name = name;
    _setTones();
}

// referenced https://stackoverflow.com/questions/39855341/equals-operator-on-stl-vector-of-pointers
bool Chord::operator==(const Chord & chord) {
    return equal(begin(this->_notes), end(this->_notes), begin(chord._notes),
                 [](Note* a, Note* b){return *a == *b;});
}

bool Chord::operator!=(const Chord & chord) {
    return !equal(begin(this->_notes), end(this->_notes), begin(chord._notes),
                 [](Note* a, Note* b){return *a == *b;});
}

void Chord::_setBass() {
    // regex to find the _bass note
    _bass.setOctave(_octave);
    regex _bassRegex = regex("(^[A-Ga-g](#|b)?)|(/[A-Ga-g](#|b)?)");
    std::sregex_iterator end;
    // augmented and diminished messages often confuse our _bass finder regex so remove temporarily
    string nameCleaned = regex_replace(_name,
                                       regex("((aug)|(Aug)|(dim)|(Dim)|(hdim)|(Hdim)).*"), "");
    for(std::sregex_iterator it(nameCleaned.begin(), nameCleaned.end(), _bassRegex);
        it != end; it++) {
        // The _bass will always be the last match. For example, the regex search of C#/Gb returns
        // C -> C# -> G -> Gb
        // If our _bass is at a slash, take out the slash
        _bass = (it->str())[0] == '/' ? (it->str()).substr(1) : it->str();
    }
}

void Chord::_setFirst() {
    _first = Note(_name.substr(0, 2), _octave);
    if(_first < _bass) {
        _first.setOctave(_bass.octave() + 1);
    }
}

void Chord::_setSecond() {
    int _secondDist = regex_search(_name, regex("(b9)|(b2)")) ? 1 : 2;
    _secondDist = regex_search(_name, regex("(#9)|(#2)")) ? 3 : _secondDist;
    _second = _first + _secondDist;
}

void Chord::_setThird() {
    int _thirdDist = regex_search(_name, regex("-|~|(^[a-gA-G](#|b)?( )?(/.*)?m(#|b| |[1-9]|(aug)|"
                                               "\\+)|m$)|^[a-g]|(b3)|(min)|(dim)|(hdim)")) ? 3 : 4;
    _thirdDist = regex_search(_name, regex("#3")) ? 5 : _thirdDist;
    _third = _first + _thirdDist;
}

void Chord::_setFourth() {
    int _fourthDist = regex_search(_name, regex("(#11)|(#4)")) ? 6 : 5;
    _fourthDist = regex_search(_name, regex("(b4)|(b11)")) ? 4 : _fourthDist;
    _fourth = _first + _fourthDist;
}

void Chord::_setFifth() {
    int _fifthDist = regex_search(_name, regex("(#5)|\\+|(aug)")) ? 8 : 7;
    _fifthDist = regex_search(_name, regex("(b5)|(dim)|(hdim)")) ? 6 : _fifthDist;
    _fifth = _first + _fifthDist;
}

void Chord::_setSixth() {
    int _sixthDist = regex_search(_name, regex("(#6)|(#13)")) ? 10 : 9;
    _sixthDist = regex_search(_name, regex("(b6)|(b13)")) ? 8 : _sixthDist;
    _sixth = _first + _sixthDist;
}

void Chord::_setSeventh() {
    int _seventhDist = regex_search(_name, regex("maj|[A-G]#?b?$")) ? 11 : 10;
    _seventhDist = regex_search(_name, regex("b7")) ? 10 : _seventhDist;
    _seventhDist = regex_search(_name, regex("[a-gA-G](#|b)?( )?(dim)")) ? 9 : _seventhDist;
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

Note Chord::bass() {
    return _bass;
}

Note Chord::first() {
    return _first;
}

Note Chord::second() {
    return _second;
}

Note Chord::third() {
    return _third;
}

Note Chord::fourth() {
    return _fourth;
}

Note Chord::fifth() {
    return _fifth;
}

Note Chord::sixth() {
    return _sixth;
}

Note Chord::seventh() {
    return _seventh;
}

Note Chord::root() {
    return first();
}

Note Chord::ninth() {
    return second();
}

Note Chord::eleventh() {
    return fourth();
}

Note Chord::thirteenth() {
    return sixth();
}
