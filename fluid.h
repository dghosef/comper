/**
 * Wrappers for several fluid functions
 */

#include <fluidsynth.h>
#include <stdexcept> // std::runtime_error
#include <string>

namespace comper {
    /**
     * Wrapper for fluid_file_renderer. Given an input midi file, an output audio file, and a soundfont,
     * renders the input to the output.
     * @param input the path to the input file
     * @param output the path to the output file
     * @param soundfont the path to the soundfont file
     * @param format the format of the output file. If left blank, tries to deduce it and falls back to 'wav'
     * @param sampleRate the sample rate of the output file. Defaults to 44100
     * @cite http://www.fluidsynth.org/api/FileRenderer.html
     */
    void renderMidi(std::string input, std::string output, std::string soundfont,
            std::string format = "auto", float sampleRate = 44100.0) {
        fluid_settings_t* settings;
        fluid_synth_t* synth;
        fluid_player_t* player;
        fluid_file_renderer_t* renderer;
        settings = new_fluid_settings();
        /* specify the file to store the audio to
         * make sure you compiled fluidsynth with libsndfile to get a real wave file
         * otherwise this file will only contain raw s16 stereo PCM */
        fluid_settings_setstr(settings, "audio.file.name", output.c_str());
        // set our file format
        fluid_settings_setstr(settings, "audio.file.type", format.c_str());
        // use number of samples processed as timing source, rather than the system timer
        fluid_settings_setstr(settings, "player.timing-source", "sample");
        // since this is a non-realtime scenario, there is no need to pin the sample data
        fluid_settings_setint(settings, "synth.lock-memory", 0);
        // Set our sample rate
        fluid_settings_setnum(settings, "synth.sample-rate", sampleRate);
        synth = new_fluid_synth(settings);
        if(!fluid_is_midifile(input.c_str())) {
            throw std::runtime_error("input is not a valid midi file");
        }
        else if(!fluid_is_soundfont(soundfont.c_str())) {
            throw std::runtime_error("`soundfont` is not a valid soundfont file");
        }
        // load soundfont
        if(fluid_synth_sfload(synth, soundfont.c_str(), true) == FLUID_FAILED) {
            throw std::runtime_error("Failed to load soundfont. Check soundfont file path");
        }
        player = new_fluid_player(synth);
        fluid_player_add(player, input.c_str());
        fluid_player_play(player);
        renderer = new_fluid_file_renderer (synth);
        while (fluid_player_get_status(player) == FLUID_PLAYER_PLAYING)
        {
            if (fluid_file_renderer_process_block(renderer) != FLUID_OK)
            {
                break;
            }
        }
        // just for sure: stop the playback explicitly and wait until finished
        fluid_player_stop(player);
        fluid_player_join(player);

        delete_fluid_file_renderer(renderer);
        delete_fluid_player(player);
        delete_fluid_synth(synth);
        delete_fluid_settings(settings);
    }
} // comper
