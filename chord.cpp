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

void Chord::_setFirst() {
    // regex to find the _first note
    _first.setOctave(_octave);
    regex _firstRegex = regex("(^[A-Ga-g](#|b)?)|(/[A-Ga-g](#|b)?)");
    std::sregex_iterator end;
    // augmented and diminished messages often confuse our _first finder regex so remove temporarily
    string nameCleaned = regex_replace(_name,
                                       regex("((aug)|(Aug)|(dim)|(Dim)|(hdim)|(Hdim)).*"), "");
    for(std::sregex_iterator it(nameCleaned.begin(), nameCleaned.end(),
                                _firstRegex); it != end; it++) {
        // The _first will always be the last match. For example, the regex search of C#/Gb returns
        // C -> C# -> G -> Gb
        // If our _first is at a slash, take out the slash
        _first = (it->str())[0] == '/' ? (it->str()).substr(1) : it->str();
    }
}

void Chord::_setBottom() {
    _bottom = Note(_name.substr(0, 2), _octave); // We need the bottom of the chord to find
                                            // each chord element. Root isn't enough b/c of / chords
    if(_bottom < _first) {
        _bottom.setOctave(_bottom.octave() + 1);
    }
}

void Chord::_setSecond() {
    int _secondDist = regex_search(_name, regex("(b9)|(b2)")) ? 1 : 2;
    _secondDist = regex_search(_name, regex("(#9)|(#2)")) ? 3 : _secondDist;
    _second = _bottom + _secondDist;
}

void Chord::_setThird() {
    int _thirdDist = regex_search(_name, regex("-|~|(^[a-gA-G](#|b)?( )?(/.*)?m(#|b| |[1-9]|(aug)|"
                                               "\\+)|m$)|^[a-g]|(b3)|(min)|(dim)|(hdim)")) ? 3 : 4;
    _thirdDist = regex_search(_name, regex("#3")) ? 5 : _thirdDist;
    _third = _bottom + _thirdDist;
}

void Chord::_setFourth() {
    int _fourthDist = regex_search(_name, regex("(#11)|(#4)")) ? 6 : 5;
    _fourthDist = regex_search(_name, regex("(b4)|(b11)")) ? 4 : _fourthDist;
    _fourth = _bottom + _fourthDist;
}

void Chord::_setFifth() {
    int _fifthDist = regex_search(_name, regex("(#5)|\\+|(aug)")) ? 8 : 7;
    _fifthDist = regex_search(_name, regex("(b5)|(dim)|(hdim)")) ? 6 : _fifthDist;
    _fifth = _bottom + _fifthDist;
}

void Chord::_setSixth() {
    int _sixthDist = regex_search(_name, regex("(#6)|(#13)")) ? 10 : 9;
    _sixthDist = regex_search(_name, regex("(b6)|(b13)")) ? 8 : _sixthDist;
    _sixth = _bottom + _sixthDist;
}

void Chord::_setSeventh() {
    int _seventhDist = regex_search(_name, regex("maj|[A-G]#?b?$")) ? 11 : 10;
    _seventhDist = regex_search(_name, regex("b7")) ? 10 : _seventhDist;
    _seventhDist = regex_search(_name, regex("[a-gA-G](#|b)?( )?(dim)")) ? 9 : _seventhDist;
    _seventh = _bottom + _seventhDist;
}

void Chord::_setTones() {
    _setFirst();
    _setBottom();
    _setSecond();
    _setThird();
    _setFourth();
    _setFifth();
    _setSixth();
    _setSeventh();
    /*
    // For each chord tone, performs a regex search for an instruction to be altered and sets it
    // accordingly
    int _secondDist = regex_search(_name, regex("(b9)|(b2)")) ? 1 : 2;
    _secondDist = regex_search(_name, regex("(#9)|(#2)")) ? 3 : _secondDist;
    _second = _bottom + _secondDist;

    int _thirdDist = regex_search(_name, regex("-|~|(^[a-gA-G](#|b)?( )?(/.*)?m(#|b| |[1-9]|(aug)|\\+)|m$)"
                                               "|^[a-g]|(b3)|(min)|(dim)|(hdim)")) ? 3 : 4;
    _thirdDist = regex_search(_name, regex("#3")) ? 5 : _thirdDist;
    _third = _bottom + _thirdDist;

    int _fourthDist = regex_search(_name, regex("(#11)|(#4)")) ? 6 : 5;
    _fourthDist = regex_search(_name, regex("(b4)|(b11)")) ? 4 : _fourthDist;
    _fourth = _bottom + _fourthDist;

    int _fifthDist = regex_search(_name, regex("(#5)|\\+|(aug)")) ? 8 : 7;
    _fifthDist = regex_search(_name, regex("(b5)|(dim)|(hdim)")) ? 6 : _fifthDist;
    _fifth = _bottom + _fifthDist;

    int _sixthDist = regex_search(_name, regex("(#6)|(#13)")) ? 10 : 9;
    _sixthDist = regex_search(_name, regex("(b6)|(b13)")) ? 8 : _sixthDist;
    _sixth = _bottom + _sixthDist;

    int _seventhDist = regex_search(_name, regex("maj|[A-G]#?b?$")) ? 11 : 10;
    _seventhDist = regex_search(_name, regex("b7")) ? 10 : _seventhDist;
    _seventhDist = regex_search(_name, regex("[a-gA-G](#|b)?( )?(dim)")) ? 9 : _seventhDist;
    _seventh = _bottom + _seventhDist;
    */
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
