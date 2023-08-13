#ifndef BAT_NOTE_H
#define BAT_NOTE_H

struct bat_note {
    uint8_t note;
    uint8_t velocity;
    uint64_t duration;  /* 1/BAT_SHORTEST_NOTE */
    double start;
    double pitch;
};


bat_note_t* bat_note_new(uint8_t note, uint8_t velocity, uint64_t duration, double start);

bat_note_t* bat_note_init(bat_note_t *self, uint8_t note, uint8_t velocity, uint64_t duration, double start);

double bat_note_to_pitch(bat_note_t *self, bat_inst_t *inst);

double bat_note_shortest_duration(bat_inst_t *inst);

double bat_note_duration(bat_note_t *self, bat_inst_t *inst);

#endif
