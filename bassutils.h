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

#ifndef BASSUTILS_H
#define BASSUTILS_H
#include <vector>
#include <map>
#include <algorithm> // std::copy_if

#include "note.h"
#include "chord.h"

/// @todo optimize everything(it could/should be much better)
/// @todo use const in the right place
namespace comper {
    enum Direction : bool {Up = true, Down = false};
    /* Finds the closest note with the same name as `original` to `comparison` in direction `dir`
     * Ex: If original is a C4 and comparison is a D5 and dir is up, we return a C6
     */
    Note closestNote(Note original, Note comparison, Direction dir) {
        original = comparison.closest(original.name());
        if(original == comparison) {
            return original;
        }
        // Make sure original is in the right direction from comparison and adjust its octave accordingly
        if((original > comparison) != dir) {
            original.setOctave(dir ? original.octave() + 1 : original.octave() - 1);
        }
        return original;
    }

    bool isBetween(const Note &original, const Note &comparison1, const Note &comparison2) {
        return std::max(comparison1.number(), comparison2.number()) > original.number() &&
                std::min(comparison1.number(), comparison2.number()) < original.number();
    }
    /**
     * Returns the closest leading note to `nextNote` from `currentNote` in the direction specified by
     * `dir`. Leading notes are: the chord tones from `currentChord` surrounding `nextNote`, the note
     * a fifth above/a fourth above `nextNote` only if that fifth also happens to be a chord tone of
     * currentChord, and the semitone above/below nextNote only if currentNote is a whole step away from
     * nextNote. nextNote's octave won't be taken into account. If nextNote is equal to current note,
     * return a fifth above/a fourth below currentNote(depending on `dir`). Sets velocity and duration equal
     * to currentNote's
     *
     * @example leadingNote(C, D, C, Up) == C#
     * @example leadingNote(E, G, C, Up) == F
     * @example leadingNote(C, C, C, Up) == G
     * @example leadingNote(E, G, C, Down) == D
     * actual note/chord objects left out for the sake of illustration
     */
    Note closestLeadingNote(Note currentNote, Note nextNote, Chord currentChord, Direction dir) {
        nextNote = closestNote(nextNote, currentNote, dir);
        // Return a fifth above/fourth below depending on dir if currentNote and nextNote are identical
        if(currentNote == nextNote) {
            return dir ? currentNote + 7 : currentNote + -5;
        }
        // If we're a whole step away, return the leading tone a semitone away from nextNote
        if(currentNote.shortestDistance(nextNote) == 2) {
            return dir ? currentNote + 1 : currentNote + -1;
        }
        /* Adjust the octaves of each chordTone to be as close as possible to currentNote in the correct
         * direction. Find the neighboring chordTone to nextNote and check if the 5th above/4th below is
         * in our chord*/
        Note fifth(__INT_MAX__, currentNote.duration(), currentNote.velocity());
        Note neighboringTone(__INT_MAX__, currentNote.duration(), currentNote.velocity());
        std::vector<Note> chordTones = currentChord.notes();
        for(auto it = chordTones.begin(); it < chordTones.end(); ++it) {
            *it = closestNote(*it, currentNote, dir);
            /* The neighboring tone is the closest chord tone to nextNote and in between currentNote
             *  and nextNote if possible */
            if(((*it - nextNote < neighboringTone - nextNote) ||
                    (isBetween(*it, currentNote, nextNote) &&
                    !isBetween(neighboringTone, currentNote, nextNote)))
                    && it->shortestDistance(nextNote) > 0 &&
                    it->shortestDistance(currentNote) > 0) {
                neighboringTone = *it;
            }
            if((it->number() - nextNote.number() == -5 || it->number() - nextNote.number() == 7) &&
                it->number() != currentNote.number()) {
                fifth = *it;
            }
        }
        //// Return the closest leading tone(fifth or neighboringTone) to currentNote
        return fifth - currentNote < neighboringTone - currentNote ? fifth : neighboringTone;
    }

    /// Returns the closest note within `currentChord`'s voicing in direction `dir`
    Note closestChordTone(const Note &currentNote, const Chord &currentChord, Direction dir) {
        std::vector<Note> voicing = currentChord.voicing();
        Note ret(__INT_MAX__, currentNote.duration(), currentNote.velocity());
        for(auto it = voicing.begin(); it < voicing.end(); ++it) {
            *it = closestNote(*it, currentNote, dir);
            if(*it - currentNote < ret - currentNote && it->number() != currentNote.number()) {
                ret = *it;
            }
        }
        return ret;
    }

    /// Returns the `n`th closest note within `currentChord` in direction `dir`
    Note nthClosestChordTone(Note currentNote, const Chord &currentChord, Direction dir, int n) {
        while(n-- > 0) {
            currentNote = closestChordTone(currentNote, currentChord, dir);
        }
        return currentNote;
    }
    
} // comper
#endif
