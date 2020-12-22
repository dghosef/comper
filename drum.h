#ifndef DRUM_H
#define DRUM_H
#include <vector>
#include <string>
#include "note_numbers.h"

#include "probcfg.h"
#include "midiwriter.h"
namespace comper {
    std::vector<Note> addSimpleDrumSwingPattern(const int measureCount, const int velocity = 100) {
        std::vector<Note> pattern;
        for(int i = 0; i < measureCount * 2; ++i) {
            /// @todo remove magic number
            pattern.push_back(Note(51, 4, velocity));
            pattern.push_back(Note(51, 8, velocity));
            pattern.push_back(Note(51, 8, velocity));
        }
        return pattern;
    }
}
#endif
