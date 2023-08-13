#include "bat.h"


bat_inst_t* bat_inst_new(uint32_t sample_rate, double pitch, uint16_t tempo, uint8_t num, uint8_t denom) {
    bat_inst_t *self;
    if (!(self = malloc(sizeof(*self)))) {
        return NULL;
    }
    return bat_inst_init(self, sample_rate, pitch, tempo, num, denom);
}

bat_inst_t* bat_inst_init(bat_inst_t *self, uint32_t sample_rate, double pitch, uint16_t tempo, uint8_t num, uint8_t denom) {
    if (!self) {
        return NULL;
    }
    self->sample_rate = sample_rate;
    self->pitch = pitch;
    self->tempo = tempo;
    self->sig_num = num;
    self->sig_denom = denom;
    return self;
}


bat_error_t bat_inst_destroy(bat_inst_t **self) {
    if (!self || !*self) {
        return BAT_ERR;
    }
    free(*self);
    *self = NULL;
    return BAT_OK;
}
