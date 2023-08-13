#include "bat.h"


bat_mod_t *bat_mod_new(size_t type, double amount, void *mod) {
    bat_mod_t *self;
    if (!mod || !(self = malloc(sizeof(*self)))) {
        return NULL;
    }
    return bat_mod_init(self, type, amount, mod);
}


bat_mod_t *bat_mod_init(bat_mod_t *self, size_t type, double amount, void *mod) {
    if (!self || !mod) {
        return NULL;
    }
    self->type = type;
    self->amount = amount;
    self->value = 0.0;
    if (type == BAT_MOD_ENV) {
        self->env = (bat_env_t*)mod;
    } else {
        self->lfo = (bat_lfo_t*)mod;
    }
    return self;
}


bat_error_t bat_mod_destroy(bat_mod_t **self) {
    if (!self || !*self) {
        return BAT_ERR;
    }
    free(*self);
    *self = NULL;
    return BAT_OK;
}


bat_error_t bat_mod_sample(bat_mod_t *self, double t) {
    if (!self) {
        return BAT_ERR;
    }
    if (self->type == BAT_MOD_ENV) {
        self->value = bat_env_sample(self->env, t);
    } else {
        self->value = bat_lfo_sample(self->lfo, t);
    }

    return BAT_OK;
}
