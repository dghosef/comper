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

#include <cstdlib>
#include <iostream> // std::cout, fstream
#include <fstream>
#include <string>
#include <vector>

#include "drum.h"
#include "comp.h"
#include "probcfg.h"
#include "simpleBassline.h"
#include "bassutils.h" // comper::quarterNoteChord
#include "probcfg.h"
#include "midiwriter.h"

/// @todo put classes in namespace, use runtime_error more or standardize approaches
/// @todo liccenses
int main(int argc, char *argv[]) {
    if(argc != 4) {
        std::cout << "usage: comper <progression file> <cfgfile> bpm" << std::endl;
        return 1;
    }
    int velocity = 100;
    std::vector<comper::quarterNoteChord> progression;
    std::ifstream progressionFile(argv[1]);
    int totalDuration = 0;
    if(progressionFile.is_open()) {
        std::string line;
        while(getline(progressionFile, line)) {
            std::string chordName = line.substr(0, line.find_last_of(' '));
            int duration = strtol(line.substr(line.find_last_of(' ') + 1).c_str(), nullptr, 10);
            Chord nextChord = comper::quarterNoteChord(chordName);
            nextChord.setDuration(duration);
            totalDuration += duration;
            nextChord.setVelocity(velocity);
            progression.push_back(nextChord);
        }
    }
    else {
        std::cout << "Illegal filename";
        return 1;
    }
    int bpm = std::strtol(argv[3], nullptr, 10);
    std::string cfgFile = std::string(argv[2]);
    MidiWriter writer(bpm, 2.0/3.0);
    ProbCFG bassPatternCFG;
    bassPatternCFG.fromFile(cfgFile, "bassPattern");
    ProbCFG bassDirectionCFG;
    bassDirectionCFG.fromFile(cfgFile, "bassDirection");
    int bassInstrumentNumber = 34;
    writer.addNotes(comper::genSimpleWalkingBassline(progression, bassPatternCFG, bassDirectionCFG,
                Note("C", 3), Note("G", 3)), bassInstrumentNumber);
    ProbCFG compingRhythmCFG;
    compingRhythmCFG.fromFile(cfgFile, "compingRhythm");
    ProbCFG compingDirectionCFG;
    compingDirectionCFG.fromFile(cfgFile, "compingDirection");
    std::vector<std::vector<int>> voicings = {{3, 6, 7, 9}, {7, 9, 3, 5}};
    int chordInstrumentNumber = 1;
    writer.addChords(comper::genComping(progression, compingRhythmCFG, compingDirectionCFG, voicings,
                Note("G", 5), velocity), chordInstrumentNumber);
    int drumInstrumentNumber = 5;
    writer.addNotes(comper::addSimpleDrumSwingPattern(totalDuration / 4), drumInstrumentNumber, true);
    writer.write("backing.mid");
    return 0;
}
