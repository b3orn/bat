#ifndef BAT_TYPES_H
#define BAT_TYPES_H

typedef size_t bat_error_t;

typedef struct bat_inst bat_inst_t;
typedef struct bat_state bat_state_t;
typedef struct bat_osc bat_osc_t;
typedef struct bat_filter bat_filter_t;
typedef struct bat_env bat_env_t;
typedef struct bat_lfo bat_lfo_t;
typedef struct bat_mod bat_mod_t;
typedef struct bat_note bat_note_t;
typedef struct bat_wav_file bat_wav_file_t;
typedef struct bat_mid_file bat_mid_file_t;
typedef struct bat_mid_event bat_mid_event_t;

typedef double (bat_osc_waveform_t)(bat_osc_t *osc, bat_note_t *note, double t);
typedef double (bat_filterfunc_t)(bat_filter_t *filter, bat_note_t *note, double sample);
typedef double (bat_lfo_waveform_t)(bat_lfo_t *lfo, double t);

#endif
