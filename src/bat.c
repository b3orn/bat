#include "bat.h"

#define N_OSC 3
#define N_NOTES 128


static void usage(char *program) {
    printf("usage: %s input output\n", program);
}


static double bat_handle_events(bat_mid_event_t *event, bat_inst_t *inst, bat_mid_file_t *mid, bat_env_t *envs, bat_note_t *notes, double dt, double t) {
    size_t n;
    double next_event;
    do {
        if (event->type == BAT_END_OF_TRACK_EVENT) {
            next_event = (double)INFINITY;  /* apparently no double precision exists */
            printf("%f stop\n", t);
            for (n = 0; n < N_NOTES; ++n) {
                if (bat_env_is_triggered(&envs[n]) && !bat_env_is_released(&envs[n])) {
                    printf("note off %d\n", notes[n].note);
                    bat_env_release(&envs[n], t);
                }
            }
            break;
        } else if (event->type == BAT_MIDI_EVENT && (event->status & 0x90) == 0x90) {
            /* note on */
            n = event->data0;
            bat_env_reset(&envs[n]);
            bat_env_trigger(&envs[n], t);
            notes[n].note = event->data0;
            notes[n].velocity = event->data1;
            notes[n].start = t;
            printf("%f note on %zu\n", t, n);
        } else if (event->type == BAT_MIDI_EVENT && (event->status & 0x80) == 0x80) {
            /* note off */
            n = event->data0;
            printf("%f note off %zu\n", t, n);
            bat_env_release(&envs[n], t);
        }

        if (bat_mid_file_next_event(mid, inst, event) != BAT_OK) {
            return -1.0;
        }
        next_event = t + dt * event->dt;
    } while (event->dt == 0);

    return next_event;
}


static int bat_play(bat_inst_t *inst, bat_osc_t osc[][N_OSC], bat_filter_t *filters, bat_env_t *envs, bat_note_t *notes, bat_mid_file_t *mid, bat_wav_file_t *wav) {
    size_t i, n, m;
    bat_mid_event_t event;
    double t, next_event, dt, sample, output, amp, last_amp;
    float output_float;

    dt = bat_note_shortest_duration(inst) * (double)BAT_SHORTEST_NOTE / (4 * mid->ticks);

    if (bat_mid_file_next_event(mid, inst, &event) != BAT_OK) {
        return 1;
    }

    t = 0.0;
    amp = 0.0;
    last_amp = 0.0;
    next_event = dt * event.dt;
    for (i = 0; !(event.type == BAT_END_OF_TRACK_EVENT && last_amp <= BAT_LOWEST_LEVEL); ++i) {
        t = i/(double)inst->sample_rate;
        output = 0.0;
        last_amp = 0.0;

        if (t >= next_event) {
            next_event = bat_handle_events(&event, inst, mid, envs, notes, dt, t);
            if (next_event < 0.0) {
                return 1;
            }
        }

        for (m = 0; m < N_NOTES; ++m) {
            if (!bat_env_is_triggered(&envs[m])) {
                continue;
            }
            sample = 0.0;
            for (n = 0; n < N_OSC; ++n) {
                if (osc[m][n].fmod) {
                    bat_mod_sample(osc[m][n].fmod, t);
                }
                sample += bat_osc_sample(&osc[m][n], &notes[m], t);
            }

            bat_mod_sample(filters[m].cmod, t);
            amp = bat_env_sample(&envs[m], t);
            output += amp * bat_filter_process(&filters[m], &notes[m], sample);

            if (amp > last_amp) {
                last_amp = amp;
            }
        }

        output_float = (float)(atan(output) * 2 / M_PI);
        if (bat_wav_file_write(wav, &output_float) != BAT_OK) {
            return 1;
        }
    }

    printf("%f stopped\n", t);

    return 0;
}


int main(int argc, char **argv) {
    double filter_buffer[N_NOTES][4];
    int status, i;
    bat_inst_t inst;
    bat_osc_t osc[N_NOTES][N_OSC];
    bat_filter_t filters[N_NOTES];
    bat_env_t envs[N_NOTES];
    bat_env_t filter_envs[N_NOTES];
    bat_lfo_t osc_lfos[N_NOTES];
    bat_mod_t filter_mods[N_NOTES];
    bat_mod_t osc_mods[N_NOTES];
    bat_note_t notes[N_NOTES];
    bat_wav_file_t wav_file;
    bat_mid_file_t mid_file;
    char *input_filename, *output_filename;
    struct timeval t_start, t_init, t_end, t_res;

    gettimeofday(&t_start, NULL);

    if (argc < 3) {
        usage(argv[0]);
        return 1;
    } else {
        input_filename = argv[1];
        output_filename = argv[2];
    }

    bat_inst_init(&inst, BAT_DEFAULT_SAMPLE_RATE, BAT_PITCH_A440, 107, 4, 4);

    for (i = 0; i < N_NOTES; ++i) {
        bat_env_init(&filter_envs[i], &inst, 128/128, 128/16, 128/2, 1, 0.25);
        bat_mod_init(&filter_mods[i], BAT_MOD_ENV, 0.25, &filter_envs[i]);
        bat_lfo_init(&osc_lfos[i], &inst, BAT_LFO_QUANTIZED, 0.0, 1.0, bat_lfo_waveform_sin, 128/1, 128/8, 0.0);
        bat_mod_init(&filter_mods[i], BAT_MOD_LFO, 0.1, &osc_lfos[i]);
        bat_filter_init(&filters[i], &inst, 2, 4, filter_buffer[i], bat_filter_lp2);
        bat_env_init(&envs[i], &inst, 128/128, 128/16, 128/2, 1, 0.25);
        bat_note_init(&notes[i], (uint8_t)i, 0, 0, 0.0);
        bat_osc_init(&osc[i][0], &inst, 0, 0, 1.0, bat_osc_waveform_saw);
        bat_osc_init(&osc[i][1], &inst, 10, 0, 0.75, bat_osc_waveform_saw);
        bat_osc_init(&osc[i][2], &inst, -1200, 0, 0.5, bat_osc_waveform_square);

        osc[i][0].fmod = &osc_mods[i];
        filters[i].cmod = &filter_mods[i];
    }

    mid_file.stream = fopen(input_filename, "rb");
    if (!mid_file.stream) {
        return 1;
    }

    wav_file.stream = fopen(output_filename, "wb");
    if (!wav_file.stream) {
        fclose(mid_file.stream);
        return 1;
    }

    status = 0;

    if (!bat_mid_file_init(&mid_file, mid_file.stream)) {
        status = 1;
        goto error;
    }

    if (!bat_wav_file_init(&wav_file, wav_file.stream, BAT_WAV_FLOAT, 1, inst.sample_rate, 8 * sizeof(float))) {
        status = 1;
        goto error;
    }

    gettimeofday(&t_init, NULL);

    status = bat_play(&inst, osc, filters, envs, notes, &mid_file, &wav_file);

    gettimeofday(&t_end, NULL);

    bat_wav_file_finish(&wav_file);

error:
    fclose(wav_file.stream);
    fclose(mid_file.stream);

    timersub(&t_init, &t_start, &t_res);
    printf("init: %ld.%06lds\n", (long int)t_res.tv_sec, (long int)t_res.tv_usec);

    timersub(&t_end, &t_init, &t_res);
    printf("play: %ld.%06lds\n", (long int)t_res.tv_sec, (long int)t_res.tv_usec);

    return status;
}
