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

#ifndef SIMPLEBASSLINE_H
#define SIMPLEBASSLINE_H
#include <vector>
#include <string>
#include <stdexcept> // std::runtime_error
#include <cctype> // isdigit

#include "chord.h"
#include "midiwriter.h"
#include "note.h"
#include "probcfg.h"
#include "note_numbers.h"
#include "bassutils.h"

namespace comper {
    std::vector<int> scaleTones = {1, 2, 3, 4, 5, 6, 7};
    std::vector<int> chordTones = {1, 3, 5, 7};
    /// Identical to regular Chord object but its duration represents duration in quarter notes
    typedef Chord quarterNoteChord;
    /**
     * @param `progression` A vector of quarterNoteChords representing the chord progression
     * @param `patternCFG` A ProbCFG that can generate a pattern. 
     *  Patterns are a string of chars. '0'-'8' mean play the corresponding chord tone of that chord.
     *  'S' means play the closest of the following chord degrees: 1, 2/9, 3, 4/11, 5, 6/13, 7
     *  'A' is the same as 'S' but with 1, 3, 5, and 7 instead
     *  'F' is the same as 'A' but we jump to the second closest chord degree instead
     *  'O' means to jump up/down an octave
     *  'R' means to repeat the last note
     * @param lowestNote the lowest our bassline will go before turning around
     *  This does not guarantee that the bassline will never go below this note, but rather, that it will
     *  start moving in the opposite direction the moment it goes past this note.
     * @param highestNote the highest our bassline will go before turning around. Opposite of lowestNote
     * @param velocity the velocity of each note in the bassline
     *
     * Generatess a vector of Notes representing a walking bassline. Generates a new pattern for each chord
     * and follows the pattern till it reaches the last beat of the chord at which point it finds the closest
     * leading note to the next root. Then it plays the closest root. Note that the root won't necessarily
     * follow the directions instruction nor will it necessarily follow highestNote nor lowestNote
     */
    std::vector<Note> genSimpleWalkingBassline(std::vector<quarterNoteChord> progression, ProbCFG patternCFG,
            ProbCFG directionCFG, Note lowestNote, Note highestNote, int velocity = 100) {
        if(lowestNote > highestNote) {
            throw std::runtime_error("LowestNote should be below highestNote");
        }
        std::vector<Note> bassline;
        Note prev;
        // Make sure at the end we have somewhere to lead to
        progression.push_back(progression[0]);
        for(auto it = progression.begin(); it < progression.end() - 1; ++it) {
            Chord currentChord = *it;
            // Generate a pattern for the current chord and the direction of each note's travel
            std::string pattern = patternCFG.generateString(currentChord.duration());
            std::string directions = directionCFG.generateString(currentChord.duration());
            if(pattern.size() < (size_t)currentChord.duration() - 1 ||
                    directions.size() < (size_t)currentChord.duration() - 1) {
                throw std::runtime_error("One of the provided CFGs produced output that was too short");
            }
            // Generate a note for each beat
            for(int i = 0; i < currentChord.duration() - 1; ++i) {
                // Begin with the first root in the progression between the limit notes.
                if(bassline.empty()) {
                    Note start = currentChord.bass();
                    start.setOctave((lowestNote.octave() + highestNote.octave()) / 2);
                    start.setVelocity(velocity);
                    start.setDuration(4);
                    bassline.push_back(start);
                    prev = start;
                    continue;
                }
                prev = *(bassline.rbegin());
                char curr = pattern[i];
                // If we go too high or too low, turn around
                if(prev.number() > highestNote.number()) {
                    directions.insert(i, "DDDD");
                } else if(prev.number() < lowestNote.number()) {
                    directions.insert(i, "UUUU");
                }
                Direction dir = (Direction)(directions[i] == 'U');
                if(isdigit(curr)) {
                    if(curr - '0' == '9') {
                        throw std::runtime_error("can only have the digits 0-8");
                    }
                    Note tone = prev.closest(currentChord.notes()[curr - '0'].name());
                    // if we're close, push back the closest, otherwise follow direction
                    if(tone - prev <= 2) {
                        bassline.push_back(tone);
                    } else {
                        bassline.push_back(closestNote(tone, prev, dir));
                    }
                } else if(curr == 'A' || curr == 'C' || curr == 'F') {
                    // play the next chord tone
                    currentChord.setVoicing(chordTones);
                    bassline.push_back(nthClosestChordTone(prev, currentChord, dir, curr == 'F' ?
                                                               2 : 1));
                } else if(curr == 'S') {
                    // play the next scale tone
                    currentChord.setVoicing(scaleTones);
                    bassline.push_back(closestChordTone(prev, currentChord, dir));
                } else if(curr == 'O') {
                    // jump an octave
                    bassline.push_back(dir ? prev + NUM_NOTES : prev + -NUM_NOTES);
                } else if(curr == 'R') {
                    // repeat the previous note
                    bassline.push_back(prev);
                } else {
                    throw std::runtime_error("Illegal character in CFG");
                }
            }
            bassline.push_back(closestLeadingNote(*(bassline.rbegin()), (it + 1)->bass(), currentChord,
                        (Direction)(directions[currentChord.duration() - 1] == 'U')));
        }
        Note finalNote = bassline[0];
        finalNote.setDuration(1);
        bassline.push_back(finalNote);
        return bassline;
    }
} // comper
#endif
