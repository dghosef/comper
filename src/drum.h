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
