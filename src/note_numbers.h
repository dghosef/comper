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

#ifndef NOTE_NUMBERS_H
#define NOTE_NUMBERS_H

#include <map>

// Map notes to midi numbers
const std::map<char, int> MIDI_NUMBERS = {{'C', 0},
                                    {'D', 2},
                                    {'E', 4},
                                    {'F', 5},
                                    {'G', 7},
                                    {'A', 9},
                                    {'B', 11}};

// Sometimes we want to map midi numbers to notes.
const std::map<int, char> MIDI_NAMES = {{0, 'C'},
                                  {2, 'D'},
                                  {4, 'E'},
                                  {5, 'F'},
                                  {7, 'G'},
                                  {9, 'A'},
                                  {11, 'B'}};

// there are 12 total notes
const int NUM_NOTES = 12;

// there are 7 degrees in a chord
const int NUM_DEGREES = 7;
// Midi starts at 0
const int MIDI_START_OCTAVE = 0;

// The octave and midi number of middle C
const int MIDDLE_C = 60;
const int MIDDLE_OCTAVE = MIDDLE_C / NUM_NOTES - NUM_NOTES * MIDI_START_OCTAVE;
#endif // NOTE_NUMBERS_H
