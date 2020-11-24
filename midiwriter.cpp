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

#include "midiwriter.h"
#include <vector>
#include <string>
#include <stdexcept>

#include "note.h"
#include "chord.h"

using namespace std;

MidiWriter::MidiWriter() {
    _bpm = 120;
    midifile.setTicksPerQuarterNote(120); // Do we need to put this after we add channels/tracks?
    _track = 0;
}

MidiWriter::MidiWriter(const int bpm) {
    midifile.setTicksPerQuarterNote(120);
    _bpm = bpm;
    _track = 0;
}

void MidiWriter::setTempo(const int bpm) {
    if(midifile.getFileDurationInTicks() > 0) {
        throw runtime_error("You can only set tempo before you add any notes");
    }
    _bpm = bpm;
}

void MidiWriter::addNotes(const vector<Note> &notes, const int instrument) {
    if(_track > 16) {
        throw runtime_error("You have too many tracks");
    }
    midifile.addTrack();
    int actionTick = 0;
    int channel = _track; // Put each channel on a separate track
    midifile.addPatchChange(_track, actionTick, channel, instrument);
    midifile.addTempo(_track, actionTick, _bpm);
    for(Note next : notes) {
        midifile.addNoteOn(_track, actionTick, channel, next.number(), next.velocity());
        actionTick += 1.0 / next.duration() * 4.0 * _tpq;
        midifile.addNoteOff(_track, actionTick, channel, next.number(), next.velocity());
    }
    ++_track;
}

void MidiWriter::addChords(const vector<Chord> &chords, const int instrument) {
    if(_track > 16) {
        throw runtime_error("you have too many tracks");
    }
    midifile.addTrack();
    int actionTick = 0;
    int channel = _track;
    midifile.addPatchChange(_track, actionTick, channel, instrument);
    midifile.addTempo(_track, actionTick, _bpm);
    for(Chord next : chords) {
        vector<Note> voicing = next.voicing();
        for(Note nextNote : voicing) {
            midifile.addNoteOn(_track, actionTick, channel, nextNote.number(), nextNote.velocity());
            midifile.addNoteOff(_track, actionTick + nextNote.duration() / 4.0 * _tpq, channel,
                    nextNote.number(), nextNote.velocity());
        }
        actionTick += next.duration() / 4.0 * _tpq;
    }
}

void MidiWriter::write(string filename) {
    midifile.sortTracks();
    midifile.write(filename);
}
