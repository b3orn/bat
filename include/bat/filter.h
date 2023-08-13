#ifndef BAT_FILTER_H
#define BAT_FILTER_H

struct bat_filter {
    bat_inst_t *inst;
    double cutoff;
    bat_filterfunc_t *func;
    size_t n;
    double *buffer;
    bat_mod_t* cmod;
};


bat_filter_t* bat_filter_new(bat_inst_t *inst, double cutoff, size_t n, bat_filterfunc_t *func);

bat_filter_t* bat_filter_init(bat_filter_t *self, bat_inst_t *inst, double cutoff, size_t n, double *buffer, bat_filterfunc_t *func);

bat_error_t bat_filter_destroy(bat_filter_t **self);

double bat_filter_process(bat_filter_t *self, bat_note_t *note, double sample);

double bat_filter_lp1(bat_filter_t *filter, bat_note_t *note, double sample);

double bat_filter_lp2(bat_filter_t *filter, bat_note_t *note, double sample);

double bat_filter_lp4(bat_filter_t *filter, bat_note_t *note, double sample);


#endif
