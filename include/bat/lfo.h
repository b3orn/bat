#ifndef BAT_LFO_H
#define BAT_LFO_H

#define BAT_LFO_QUANTIZED 1
#define BAT_LFO_UNQUANTIZED 2

struct bat_lfo {
    bat_inst_t *inst;
    size_t type;
    double phase;
    double amp;
    bat_lfo_waveform_t *waveform;
    union {
        struct bat_lfo_quantized {
            uint8_t num;
            uint8_t denom;
        } quantized;
        struct bat_lfo_unquantized {
            double frequency;
        } unquantized;
    };
};


bat_lfo_t* bat_lfo_new(bat_inst_t *inst, size_t type, double phase, double amp, bat_lfo_waveform_t *waveform, uint8_t qnum, uint8_t qdenom, double freq);

bat_lfo_t* bat_lfo_init(bat_lfo_t *self, bat_inst_t *inst, size_t type, double phase, double amp, bat_lfo_waveform_t *waveform, uint8_t qnum, uint8_t qdenom, double freq);

bat_error_t bat_lfo_destroy(bat_lfo_t **self);

double bat_lfo_sample(bat_lfo_t *self, double t);

double bat_lfo_waveform_sin(bat_lfo_t *lfo, double t);

double bat_lfo_waveform_square(bat_lfo_t *lfo, double t);

double bat_lfo_waveform_saw(bat_lfo_t *lfo, double t);

double bat_lfo_waveform_triangle(bat_lfo_t *lfo, double t);

double bat_lfo_waveform_noise(bat_lfo_t *lfo, double t);

#endif
