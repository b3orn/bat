#ifndef BAT_MID_H
#define BAT_MID_H

#define BAT_NONE_EVENT 0
#define BAT_MIDI_EVENT 1
#define BAT_SYSEX_EVENT 2
#define BAT_END_OF_TRACK_EVENT 3

struct bat_mid_file {
    FILE *stream;
    uint16_t ticks;
    int64_t size;
};


struct bat_mid_event {
    uint64_t dt;
    size_t type;
    uint8_t status;
    uint8_t data0;
    uint8_t data1;
};


bat_mid_file_t* bat_mid_file_new(FILE *stream);

bat_mid_file_t* bat_mid_file_init(bat_mid_file_t *self, FILE *stream);

bat_error_t bat_mid_file_destroy(bat_mid_file_t **self);

bat_error_t bat_mid_file_next_event(bat_mid_file_t *self, bat_inst_t *inst, bat_mid_event_t *event);


#endif
