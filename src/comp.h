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

#ifndef COMP_H
#define COMP_H

#include <vector>
#include <string>
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::accumulate, std::min
#include <cctype>    // std::isalpha

#include "note.h"
#include "chord.h"
#include "probcfg.h"
#include "simpleBassline.h" // std::quarterNoteChord
#include "bassutils.h"      // std::Direction

namespace comper {
    /**
     * Compares each note of each chord's voicing and finds the distance between each in 
     * sequential order. Returns the largest such distance. If one voicing is longer than the other, ignores
     * the notes towards the end of the longer one that don't have a corresponding note in the other chord.
     */
    int voicingDistance(Chord original, Chord compare) {
        std::vector<Note> voicing1 = original.voicing();
        std::vector<Note> voicing2 = compare.voicing();
        int distance = voicing1[0] - voicing2[0];
        for(size_t i = 0; i < std::min(voicing1.size(), voicing2.size()); ++i) {
            distance = voicing1[i] - voicing2[i] > distance ? voicing1[i] - voicing2[i] : distance;
        }
        return distance;
    }

    /**
     * Picks the voicing out of `voicings` that facilitates the smoothest voice leading(shortest distance
     * between top notes of voicing in direction `dir`) where `original` is the chord whose voicing is being
     * altered and `compare` is the chord used to compare to.
     */
    void voiceLead(Chord &original, const Chord &compare, std::vector<std::vector<int>> voicings, Direction dir) {
        int shortestDistance = 24; // Once we set octave, distance is guaranteed to be less than 24
        std::vector<int> bestVoicing;
        int bestOctave = original.octave();
        for(std::vector<int> voicing : voicings) {
            original.setVoicing(voicing);
            Note endOfOriginal = *(original.voicing().rbegin());
            Note endOfComp = *(compare.voicing().rbegin());
            Note closest = endOfComp.closest(endOfOriginal.name());
            original.setOctave(original.octave() + closest.octave() - endOfOriginal.octave());
            if((original.voicing().rbegin()->number() > compare.voicing().rbegin()->number()) != dir &&
                    original.voicing().rbegin()->number() != compare.voicing().rbegin()->number()){
                original.setOctave(dir ? original.octave() + 1 : original.octave() - 1);
            }
            if(*(original.voicing().rbegin()) - *(compare.voicing().rbegin()) < shortestDistance){
                shortestDistance = *(original.voicing().rbegin()) - *(compare.voicing().rbegin());
                bestVoicing = voicing;
                bestOctave = original.octave();
            }
        }
        original.setVoicing(bestVoicing);
        original.setOctave(bestOctave);
    }

    /**
     * Given a progression, a rhythmCFG that produces a rhythm as specified in the README, a directionCFG
     * that produces a direction string as specified in the README, a vector of possible voicings,
     * a reference note to start voiceleading from, and the velocity, generate a comping pattern and return
     * it
     */
    std::vector<Chord> genComping(std::vector<quarterNoteChord> progression, ProbCFG rhythmCFG, 
            ProbCFG directionCFG, std::vector<std::vector<int>> voicings, Note referenceNote,
            int velocity = 100) {
        Chord referenceChord = Chord(referenceNote.name(), referenceNote.octave(), {1});
        int totalDuration = std::accumulate(progression.begin(), progression.end(), 0, 
                [](int total, const quarterNoteChord &chord) {return total + chord.duration();});
        // Change these 2 so its more efficient(less than totalDuration + 1)
        std::string rhythm = rhythmCFG.generateString(totalDuration + 1);
        std::string directions = directionCFG.generateString(totalDuration + 1);
        int rhythmIndex = 0;
        int durationSoFarInCurrentChord = 0; // in eighth notes
        int durationSoFarInProgression = 0; // in quarter notes
        int directionIndex = 0;
        std::vector<Chord> ret;
        for(auto it = progression.begin(); it < progression.end(); ++it) {
            Direction nextDirection = (Direction)(directions[directionIndex++] == 'U');
            if(durationSoFarInProgression % 16 == 0) {
                // reset octave every 4 bars
                voiceLead(*it, referenceChord, voicings, Down);
            } else {
                voiceLead(*it, *(it - 1), voicings, nextDirection);
            }
            while(durationSoFarInCurrentChord < it->duration() * 2) { // Convert to eighth notes
                int nextChordDuration;
                char nextRhythm = rhythm[rhythmIndex++];
                bool isRest = isalpha(nextRhythm);
                if(nextRhythm == '4' || nextRhythm == 'q') {
                    nextChordDuration = 4;
                } else if(nextRhythm == '8' || nextRhythm == 'e') {
                    nextChordDuration = 8;
                } else if(nextRhythm == '2') {
                    nextChordDuration = 2;
                } else if(nextRhythm == '1') {
                    nextChordDuration = 1;
                } else {
                    std::string errorMessage = "Illegal character generated by CFG: ";
                    errorMessage += nextRhythm;
                    throw std::runtime_error(errorMessage);
                }
                durationSoFarInCurrentChord += 8 / nextChordDuration;
                Chord nextChord = *it;
                nextChord.setDuration(nextChordDuration);
                nextChord.setVelocity(isRest ? 0 : velocity);
                ret.push_back(nextChord);
            }
            durationSoFarInCurrentChord =  durationSoFarInCurrentChord - it->duration() * 2 ;
            durationSoFarInProgression += it->duration();
        }
        progression[0].setVelocity(velocity);
        progression[0].setDuration(1);
        ret.push_back(progression[0]);
        return ret;
    }
}
#endif // COMP_H
