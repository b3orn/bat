#ifndef BAT_INST_H
#define BAT_INST_H

struct bat_inst {
    uint32_t sample_rate;
    double pitch;
    uint16_t tempo;
    uint8_t sig_num;
    uint8_t sig_denom;  /* 1, 2, 4, 8, 16, 32,... */
};


bat_inst_t* bat_inst_new(uint32_t sample_rate, double pitch, uint16_t tempo, uint8_t num, uint8_t denom);

bat_inst_t* bat_inst_init(bat_inst_t *self, uint32_t sample_rate, double pitch, uint16_t tempo, uint8_t num, uint8_t denom);

bat_error_t bat_inst_destroy(bat_inst_t **self);


#endif
