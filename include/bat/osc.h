#ifndef BAT_OSC_H
#define BAT_OSC_H

struct bat_osc {
    bat_inst_t *inst;
    int16_t tune;  /* cents */
    double phase;  /* [-pi;pi] */
    double amp;
    bat_mod_t *fmod;
    bat_osc_waveform_t *waveform;
};


bat_osc_t* bat_osc_new(bat_inst_t *inst, int16_t tune, double phase, double amp, bat_osc_waveform_t *waveform);

bat_osc_t* bat_osc_init(bat_osc_t *self, bat_inst_t *inst, int16_t tune, double phase, double amp, bat_osc_waveform_t *waveform);

bat_error_t bat_osc_destroy(bat_osc_t **self);

double bat_osc_sample(bat_osc_t *self, bat_note_t *note, double t);

double bat_osc_waveform_sin(bat_osc_t *osc, bat_note_t *note, double t);

double bat_osc_waveform_square(bat_osc_t *osc, bat_note_t *note, double t);

double bat_osc_waveform_saw(bat_osc_t *osc, bat_note_t *note, double t);

double bat_osc_waveform_triangle(bat_osc_t *osc, bat_note_t *note, double t);

double bat_osc_waveform_noise(bat_osc_t *osc, bat_note_t *note, double t);


#endif
