#include "bat.h"


bat_note_t* bat_note_new(uint8_t note, uint8_t velocity, uint64_t duration, double start) {
    bat_note_t *self;
    if(!(self = malloc(sizeof(*self)))) {
        return NULL;
    }
    return bat_note_init(self, note, velocity, duration, start);
}


bat_note_t* bat_note_init(bat_note_t *self, uint8_t note, uint8_t velocity, uint64_t duration, double start) {
    if (!self) {
        return NULL;
    }
    self->note = note;
    self->velocity = velocity;
    self->duration = duration;
    self->start = start;
    self->pitch = -1.0;
    return self;
}


double bat_note_to_pitch(bat_note_t *self, bat_inst_t *inst) {
    if (!self || !inst) {
        return 0.0;
    }
    if (self->pitch <= 0.0) {
        self->pitch = inst->pitch * pow(2, (self->note - BAT_MIDDLE_A)/(double)12.0);
    }
    return self->pitch;
}


double bat_note_shortest_duration(bat_inst_t *inst) {
    double beat_duration, whole_note;
    if (!inst) {
        return 0.0;
    }
    beat_duration = (double)60 / inst->tempo;
    whole_note = beat_duration * inst->sig_denom;
    return whole_note / BAT_SHORTEST_NOTE;
}


double bat_note_duration(bat_note_t *self, bat_inst_t *inst) {
    if (!self || !inst) {
        return 0.0;
    }
    return bat_note_shortest_duration(inst) * self->duration;
}
