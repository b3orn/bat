#include "bat.h"


bat_lfo_t* bat_lfo_new(bat_inst_t *inst, size_t type, double phase, double amp, bat_lfo_waveform_t *waveform, uint8_t num, uint8_t denom, double freq) {
    bat_lfo_t *self;
    if (!inst || !(self = malloc(sizeof(*self)))) {
        return NULL;
    }
    return bat_lfo_init(self, inst, type, phase, amp, waveform, num, denom, freq);
}


bat_lfo_t* bat_lfo_init(bat_lfo_t *self, bat_inst_t *inst, size_t type, double phase, double amp, bat_lfo_waveform_t *waveform, uint8_t num, uint8_t denom, double freq) {
    if (!self || !inst || !waveform) {
        return NULL;
    }
    self->inst = inst;
    self->type = type;
    self->phase = phase;
    self->amp = amp;
    self->waveform = waveform;

    if (type == BAT_LFO_QUANTIZED) {
        self->quantized.num = num;
        self->quantized.denom = denom;
    } else {
        self->unquantized.frequency = freq;
    }

    return self;
}


bat_error_t bat_lfo_destroy(bat_lfo_t **self) {
    if (!self || *self) {
        return BAT_ERR;
    }
    free(*self);
    *self = NULL;
    return BAT_OK;
}


double bat_lfo_sample(bat_lfo_t *self, double t) {
    if (!self) {
        return 0.0;
    }
    return self->waveform(self, t);
}


static double bat_lfo_frequency(bat_lfo_t *lfo) {
    double duration;
    if (!lfo) {
        return 0.0;
    }
    if (lfo->type == BAT_LFO_QUANTIZED) {
        duration = bat_note_shortest_duration(lfo->inst) * BAT_SHORTEST_NOTE;
        return (double)lfo->quantized.denom / (duration * lfo->quantized.num);
    }
    return lfo->unquantized.frequency;
}


double bat_lfo_waveform_sin(bat_lfo_t *lfo, double t) {
    if (!lfo) {
        return 0.0;
    }
    return lfo->amp * sin(2 * M_PI * bat_lfo_frequency(lfo) * t + lfo->phase);
}


double bat_lfo_waveform_square(bat_lfo_t *lfo, double t) {
    if (!lfo) {
        return 0.0;
    }
    return 2 * lfo->amp * (0.5 - signbit(bat_lfo_waveform_saw(lfo, t)));
}


double bat_lfo_waveform_saw(bat_lfo_t *lfo, double t) {
    double p;
    if (!lfo) {
        return 0.0;
    }
    p = 1/bat_lfo_frequency(lfo);
    return 2 * lfo->amp * (fmod(t, p) - p/2)/p;
}


double bat_lfo_waveform_triangle(bat_lfo_t *lfo, double t) {
    double p;
    if (!lfo) {
        return 0.0;
    }
    p = 1/bat_lfo_frequency(lfo);
    return 2 * lfo->amp * (2 * fabs(t/p - floor(t/p + 0.5)) - 0.5);
}


double bat_lfo_waveform_noise(bat_lfo_t *lfo, double t) {
    double f;
    unsigned int seed;
    int r;
    if (!lfo) {
        return 0.0;
    }
    f = bat_lfo_frequency(lfo);
    seed = (unsigned int)(t * lfo->inst->sample_rate + lfo->inst->pitch + f + lfo->phase);
    r = rand_r(&seed);
    return 2 * lfo->amp * ((double)r - (double)RAND_MAX/2)/RAND_MAX;
}
