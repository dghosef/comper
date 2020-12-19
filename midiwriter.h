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
#ifndef MIDIWRITER_H
#define MIDIWRITER_H
#include <string>
#include <vector>

#include "note.h"
#include "chord.h"
#include "midifile/MidiFile.h"

/// Writes Notes and Chords to midi files. Only supports quarter notes and eighth notes when we swing
class MidiWriter {
public:
    /// Default constructor. Sets tempo to 120
    MidiWriter();

    /// Constructor that sets tempo to `bpm`.
    MidiWriter(const int bpm);

    /// @param swing the ratio of the duration of the first eighth note to the second
    MidiWriter(const int bpm, const double swing);

    /// Adds a line of notes to the beginning of our midi file
    void addNotes(const std::vector<Note> &notes, const int instrument, bool drum = false);

    /// Adds a line of chords to the beginning of our midi file
    void addChords(const std::vector<Chord> &chords, const int instrument);

    /// Writes our midi data to `fileName`
    void write(const std::string fileName);

private:
    // The object that stores all our midi data
    smf::MidiFile midifile;

    // The bpm of our midi file
    int _bpm;

    // Tracks the track number we write to
    int _track;

    // The number of ticks per quarter note
    int _tpq;

    // The ratio of the duration of the first eighth note to the second
    double _swing;
};
#endif
