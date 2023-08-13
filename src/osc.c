#include "bat.h"


bat_osc_t* bat_osc_new(bat_inst_t *inst, int16_t tune, double phase, double amp, bat_osc_waveform_t *waveform) {
    bat_osc_t *self;
    if (!inst || !waveform || !(self = malloc(sizeof(*self)))) {
        return NULL;
    }
    return bat_osc_init(self, inst, tune, phase, amp, waveform);
}


bat_osc_t* bat_osc_init(bat_osc_t *self, bat_inst_t *inst, int16_t tune, double phase, double amp, bat_osc_waveform_t *waveform) {
    if (!self || !inst || !waveform) {
        return NULL;
    }
    self->inst = inst;
    self->tune = tune;
    self->phase = phase;
    self->amp = amp;
    self->waveform = waveform;
    self->fmod = NULL;
    return self;
}


bat_error_t bat_osc_destroy(bat_osc_t **self) {
    if (!self || !*self) {
        return BAT_ERR;
    }
    free(*self);
    *self = NULL;
    return BAT_OK;
}


double bat_osc_sample(bat_osc_t *self, bat_note_t *note, double t) {
    if (!self) {
        return 0.0;
    }
    return self->waveform(self, note, t - note->start);
}


double bat_osc_waveform_sin(bat_osc_t *osc, bat_note_t *note, double t) {
    double f;
    if (!osc || !note) {
        return 0.0;
    }
    f = bat_note_to_pitch(note, osc->inst) * pow(2, (double)osc->tune/1200.0);
    return osc->amp * sin(2 * M_PI * f * t + osc->phase);
}


double bat_osc_waveform_square(bat_osc_t *osc, bat_note_t *note, double t) {
    if (!osc || !note) {
        return 0.0;
    }
    return 2 * osc->amp * (0.5 - signbit(bat_osc_waveform_saw(osc, note, t)));
}


double bat_osc_waveform_saw(bat_osc_t *osc, bat_note_t *note, double t) {
    double f, p;
    if (!osc || !note) {
        return 0.0;
    }
    f = bat_note_to_pitch(note, osc->inst) * pow(2, (double)osc->tune/1200.0);
    if (osc->fmod) {
        f *= pow(2, osc->fmod->amount * osc->fmod->value/12);
    }
    p = 1/f;
    return 2 * osc->amp * (fmod(t, p) - p/2)/p;
}


double bat_osc_waveform_triangle(bat_osc_t *osc, bat_note_t *note, double t) {
    double f, p;
    if (!osc || !note) {
        return 0.0;
    }
    f = bat_note_to_pitch(note, osc->inst) * pow(2, (double)osc->tune/1200.0);
    p = 1/f;
    return 2 * osc->amp * (2 * fabs(t/p - floor(t/p + 0.5)) - 0.5);
}


double bat_osc_waveform_noise(bat_osc_t *osc, bat_note_t *note, double t) {
    unsigned int seed;
    int r;
    (void)note;
    if (!osc) {
        return 0.0;
    }

    seed = (unsigned int)(t * osc->inst->sample_rate + osc->inst->pitch + osc->tune + osc->phase);
    r = rand_r(&seed);

    return 2 * osc->amp * ((double)r - (double)RAND_MAX/2)/RAND_MAX;
}
