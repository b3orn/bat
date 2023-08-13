#ifndef BAT_MOD_H
#define BAT_MOD_H

#define BAT_MOD_ENV 1
#define BAT_MOD_LFO 2

struct bat_mod {
    size_t type;
    double amount;
    double value;
    union {
        bat_env_t *env;
        bat_lfo_t *lfo;
    };
};


bat_mod_t *bat_mod_new(size_t type, double amount, void *mod);

bat_mod_t *bat_mod_init(bat_mod_t *self, size_t type, double amount, void *mod);

bat_error_t bat_mod_destroy(bat_mod_t **self);

bat_error_t bat_mod_sample(bat_mod_t *self, double t);

#endif
