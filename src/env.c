#include "bat.h"


bat_env_t* bat_env_new(bat_inst_t *inst, uint64_t attack, uint64_t decay, uint64_t release, double attack_level, double sustain) {
    bat_env_t *self;
    if (!inst || !(self = malloc(sizeof(*self)))) {
        return NULL;
    }
    return bat_env_init(self, inst, attack, decay, release, attack_level, sustain);
}


bat_env_t* bat_env_init(bat_env_t *self, bat_inst_t *inst, uint64_t attack, uint64_t decay, uint64_t release, double attack_level, double sustain) {
    if (!self || !inst) {
        return NULL;
    }
    self->inst = inst;
    self->attack = attack;
    self->decay = decay;
    self->release = release;
    self->attack_level = attack_level;
    self->sustain = sustain;
    self->triggered = -1.0;
    self->released = -1.0;
    self->released_level = -1.0;
    return self;
}


bat_error_t bat_env_destroy(bat_env_t **self) {
    if (!self || !*self) {
        return BAT_ERR;
    }
    free(*self);
    *self = NULL;
    return BAT_OK;
}


bool bat_env_is_triggered(bat_env_t *self) {
    if (!self || self->triggered < 0.0) {
        return false;
    }
    return true;
}


bool bat_env_is_released(bat_env_t *self) {
    if (!self || self->released < 0.0) {
        return false;
    }
    return true;
}


bat_error_t bat_env_trigger(bat_env_t *self, double t) {
    if (!self) {
        return BAT_ERR;
    }
    self->triggered = t;
    return BAT_OK;
}


bat_error_t bat_env_release(bat_env_t *self, double t) {
    if (!self) {
        return BAT_ERR;
    } else if (!bat_env_is_triggered(self)) {
        return BAT_OK;
    }
    self->released = t;
    return BAT_OK;
}


bat_error_t bat_env_reset(bat_env_t *self) {
    if (!self) {
        return BAT_ERR;
    }
    self->triggered = -1.0;
    self->released = -1.0;
    self->released_level = -1.0;
    return BAT_OK;
}


static double bat_env_attack_level(bat_env_t *self, double t) {
    double dt;
    if (!self || !bat_env_is_triggered(self)) {
        return 0.0;
    }
    dt = bat_note_shortest_duration(self->inst);
    return (self->attack_level / (dt * self->attack)) * t;
}


static double bat_env_decay_level(bat_env_t *self, double t) {
    double dt, s, a;
    if (!self || !bat_env_is_triggered(self)) {
        return 0.0;
    }
    dt = bat_note_shortest_duration(self->inst);
    a = self->attack_level;
    s = self->sustain;
    if (self->sustain <= BAT_LOWEST_LEVEL) {
        s = BAT_LOWEST_LEVEL;
    }
    if (self->attack_level <= BAT_LOWEST_LEVEL) {
        a = BAT_LOWEST_LEVEL;
    }
    return a * exp((t - self->attack * dt) / (self->decay * dt) * log(s / a));
}


static double bat_env_release_level(bat_env_t *self, double t) {
    double dt, rt, reldt, rel, att, lev;
    if (!self || !bat_env_is_triggered(self) || !bat_env_is_released(self)) {
        return 0.0;
    }
    dt = bat_note_shortest_duration(self->inst);
    rt = self->released - self->triggered;
    reldt = t - rt;
    rel = self->release * dt;
    att = dt * self->attack;

    if (reldt > rel) {
        return 0.0;
    }

    if (self->released_level < 0.0) {
        if (t <= att) {
            lev = bat_env_attack_level(self, rt);
        } else if (t - att < dt * self->decay) {
            lev = bat_env_decay_level(self, rt);
        } else {
            lev = self->sustain;
        }
        self->released_level = lev;
    } else {
        lev = self->released_level;
    }

    if (lev <= BAT_LOWEST_LEVEL) {
        return 0.0;
    }

    return lev * exp(reldt / rel * log(BAT_LOWEST_LEVEL / lev));
}


double bat_env_sample(bat_env_t *self, double t) {
    double dt, tt, att;
    if (!self || !bat_env_is_triggered(self)) {
        return 0.0;
    }
    dt = bat_note_shortest_duration(self->inst);
    tt = t - self->triggered;
    att = dt * self->attack;

    if (bat_env_is_released(self)) {
        return bat_env_release_level(self, tt);
    } else if (tt <= att) {
        return bat_env_attack_level(self, tt);
    } else if (tt - att < dt * self->decay) {
        return bat_env_decay_level(self, tt);
    }
    return self->sustain;
}
