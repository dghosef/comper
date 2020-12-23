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

#include <vector>
#include <string>
#include <stdexcept> // std::runtime_error

#include "midiwriter.h"
#include "note.h"
#include "chord.h"

MidiWriter::MidiWriter() {
    _bpm = 120;
    midifile.setTicksPerQuarterNote(120); // Do we need to put this after we add channels/tracks?
    _track = 0;
    _tpq = 120;
    _swing = 1;
}

MidiWriter::MidiWriter(const int bpm) {
    if(bpm <= 0) {
        throw std::runtime_error("cannot have a negative or 0 bpm");
    }
    midifile.setTicksPerQuarterNote(120);
    _bpm = bpm;
    _track = 0;
    _tpq = 120;
    _swing = 1;
}

MidiWriter::MidiWriter(const int bpm, const double swing) {
    midifile.setTicksPerQuarterNote(120);
    if(swing < 0.5 || swing >= 1) {
        throw std::runtime_error("swing must be between 0.5(inclusive) and 1(exclusive)");
    } else if(bpm <= 0) {
        throw std::runtime_error("cannot have a negative or 0 bpm");
    }
    _bpm = bpm;
    _track = 0;
    _tpq = 120;
    _swing = swing;
}

/// @todo remove magic numbers
void MidiWriter::addNotes(const std::vector<Note> &notes, const int instrument, bool drum) {
    if(_track == 16) {
        throw std::runtime_error("You have too many tracks");
    }
    midifile.addTrack();
    int actionTick = 0;
    int channel;
    if(drum) channel = 9; // Drum tracks are always on channel 9
    else channel = _track < 9 ? _track : _track + 1; // Put each track on a separate channel and avoid 10
    midifile.addPatchChange(_track, actionTick, channel, instrument);
    midifile.addTempo(_track, actionTick, _bpm);
    for(auto it = notes.begin(); it < notes.end(); ++it) {
        if(it->duration() == 0) {
            throw std::runtime_error("Cannot add a note with duration 0");
        }
        midifile.addNoteOn(_track, actionTick, channel, it->number(), it->velocity());
        int swingTicks = 0;
        if(actionTick % _tpq == 0 && it->duration() == 8 && (it + 1)->duration() == 8) {
            swingTicks = _swing * _tpq - 0.5 * _tpq;
        } else if(actionTick % _tpq != 0 && it->duration() == 8 && (it - 1)->duration() == 8) {
            swingTicks = -1 * (_swing * _tpq - 0.5 * _tpq);
        }
        actionTick += 4.0 / it->duration() * _tpq + swingTicks;
        midifile.addNoteOff(_track, actionTick, channel, it->number(), it->velocity());
    }
    ++_track;
}

void MidiWriter::addChords(const std::vector<Chord> &chords, const int instrument) {
    if(_track == 16) {
        throw std::runtime_error("you have too many tracks");
    }
    midifile.addTrack();
    int actionTick = 0;
    int channel = _track;
    midifile.addPatchChange(_track, actionTick, channel, instrument);
    midifile.addTempo(_track, actionTick, _bpm);
    for(auto it = chords.begin(); it < chords.end(); ++it) {
        int swingTicks = 0;
        if(actionTick % _tpq == 0 && it->duration() == 8 && (it + 1)->duration() == 8) {
            swingTicks = _swing * _tpq - 0.5 * _tpq;
        } else if(actionTick % _tpq != 0 && it->duration() == 8 && (it - 1)->duration() == 8) {
            swingTicks = -1 * (_swing * _tpq - 0.5 * _tpq);
        }
        std::vector<Note> voicing = it->voicing();
        for(Note nextNote : voicing) {
            if(nextNote.duration() == 0) {
                throw std::runtime_error("Cannot add a note with duration 0");
            }
            midifile.addNoteOn(_track, actionTick, channel, nextNote.number(), nextNote.velocity());
            midifile.addNoteOff(_track, actionTick + 4.0 / it->duration() * _tpq + swingTicks, channel,
                    nextNote.number(), nextNote.velocity());
        }
        actionTick += 4.0 / it->duration() * _tpq + swingTicks;
    }
}

void MidiWriter::write(const std::string filename) {
    midifile.sortTracks();
    midifile.write(filename);
}
