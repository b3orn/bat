#ifndef BAT_WAV_H
#define BAT_WAV_H

#define BAT_WAV_PCM 1
#define BAT_WAV_FLOAT 3

struct bat_wav_file {
    FILE *stream;
    uint32_t size;
    uint16_t format;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t frame_rate;
    uint16_t frame_size;
    uint16_t channel_size;
    uint16_t bit_rate;
};


bat_wav_file_t* bat_wav_file_new(FILE *stream, uint16_t format, uint16_t channels, uint32_t sample_rate, uint16_t bit_rate);

bat_wav_file_t* bat_wav_file_init(bat_wav_file_t *self, FILE *stream, uint16_t format, uint16_t channels, uint32_t sample_rate, uint16_t bit_rate);

bat_error_t bat_wav_file_destroy(bat_wav_file_t **self);

bat_error_t bat_wav_file_finish(bat_wav_file_t *self);

bat_error_t bat_wav_file_write(bat_wav_file_t *self, void *frame);


#endif

