#include "bat.h"


bat_filter_t* bat_filter_new(bat_inst_t *inst, double cutoff, size_t n, bat_filterfunc_t *func) {
    bat_filter_t *self;
    double *buffer;
    if (!inst || !(self = malloc(sizeof(*self) + n * sizeof(*buffer)))) {
        return NULL;
    }
    buffer = (double*)(self + sizeof(*self));
    return bat_filter_init(self, inst, cutoff, n, buffer, func);
}


bat_filter_t* bat_filter_init(bat_filter_t *self, bat_inst_t *inst, double cutoff, size_t n, double *buffer, bat_filterfunc_t *func) {
    if (!self || !inst) {
        return NULL;
    }
    self->inst = inst;
    self->cutoff = cutoff;
    self->n = n;
    self->buffer = memset(buffer, 0, n * sizeof(*buffer));
    self->func = func;
    self->cmod = NULL;
    return self;
}


bat_error_t bat_filter_destroy(bat_filter_t **self) {
    if (!self || !*self) {
        return BAT_ERR;
    }
    free(*self);
    *self = NULL;
    return BAT_OK;
}


double bat_filter_process(bat_filter_t *self, bat_note_t *note, double sample) {
    if (!self || !note) {
        return BAT_ERR;
    }
    return self->func(self, note, sample);
}


double bat_filter_lp1(bat_filter_t *filter, bat_note_t *note, double sample) {
    double cutoff, dt, x, y0;
    if (!filter || !note || filter->n < 1) {
        return 0.0;
    }
    if (filter->cmod) {
        cutoff = (filter->cutoff + filter->cmod->value) * bat_note_to_pitch(note, filter->inst);
    } else {
        cutoff = filter->cutoff * bat_note_to_pitch(note, filter->inst);
    }
    dt = 1/(double)filter->inst->sample_rate;
    y0 = filter->buffer[0];

    x = y0 + (sample - y0) * 2 * M_PI * cutoff * dt;
    filter->buffer[0] = x;

    return x;
}


double bat_filter_lp2(bat_filter_t *filter, bat_note_t *note, double sample) {
    double cutoff, dt, y0, y1, x;
    if (!filter || !note || filter->n < 2) {
        return 0.0;
    }
    if (filter->cmod) {
        cutoff = (filter->cutoff + filter->cmod->value) * bat_note_to_pitch(note, filter->inst);
    } else {
        cutoff = filter->cutoff * bat_note_to_pitch(note, filter->inst);
    }
    dt = 1/(double)filter->inst->sample_rate;
    y0 = filter->buffer[0];
    y1 = filter->buffer[1];

    x = y0 + (sample - y0) * 2 * M_PI * cutoff * dt;
    filter->buffer[0] = x;

    x = y1 + (x - y1) * 2 * M_PI * cutoff * dt;
    filter->buffer[1] = x;

    return x;
}


double bat_filter_lp4(bat_filter_t *filter, bat_note_t *note, double sample) {
    double cutoff, dt, y0, y1, y2, y3, x;
    if (!filter || !note || filter->n < 4) {
        return 0.0;
    }
    if (filter->cmod) {
        cutoff = (filter->cutoff + filter->cmod->value) * bat_note_to_pitch(note, filter->inst);
    } else {
        cutoff = filter->cutoff * bat_note_to_pitch(note, filter->inst);
    }
    dt = 1/((double)filter->inst->sample_rate);
    y0 = filter->buffer[0];
    y1 = filter->buffer[1];
    y2 = filter->buffer[2];
    y3 = filter->buffer[3];

    x = y0 + (sample - y0) * 2 * M_PI * cutoff * dt;
    filter->buffer[0] = x;

    x = y1 + (x - y1) * 2 * M_PI * cutoff * dt;
    filter->buffer[1] = x;

    x = y2 + (x - y2) * 2 * M_PI * cutoff * dt;
    filter->buffer[2] = x;

    x = y3 + (x - y3) * 2 * M_PI * cutoff * dt;
    filter->buffer[3] = x;

    return x;
}
