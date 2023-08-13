#ifndef BAT_ENV_H
#define BAT_ENV_H

struct bat_env {
    bat_inst_t *inst;
    uint64_t attack;
    uint64_t decay;
    uint64_t release;
    double attack_level;
    double sustain;
    double triggered;
    double released;
    double released_level;
};


bat_env_t* bat_env_new(bat_inst_t *inst, uint64_t attack, uint64_t decay, uint64_t release, double attack_level, double sustain);

bat_env_t* bat_env_init(bat_env_t *self, bat_inst_t *inst, uint64_t attack, uint64_t decay, uint64_t release, double attack_level, double sustain);

bat_error_t bat_env_destroy(bat_env_t **self);

bool bat_env_is_triggered(bat_env_t *self);

bool bat_env_is_released(bat_env_t *self);

bat_error_t bat_env_trigger(bat_env_t *self, double t);

bat_error_t bat_env_release(bat_env_t *self, double t);

bat_error_t bat_env_reset(bat_env_t *self);

double bat_env_sample(bat_env_t *self, double t);


#endif
