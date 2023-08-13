#include "bat.h"


bat_wav_file_t* bat_wav_file_new(FILE *stream, uint16_t format, uint16_t channels, uint32_t sample_rate, uint16_t bit_rate) {
    bat_wav_file_t *self;
    if (!stream || !(self = malloc(sizeof(*self)))) {
        return NULL;
    }
    
    if (!bat_wav_file_init(self, stream, format, channels, sample_rate, bit_rate)) {
        bat_wav_file_destroy(&self);
        return NULL;
    }

    return self;
}


bat_wav_file_t* bat_wav_file_init(bat_wav_file_t *self, FILE *stream, uint16_t format, uint16_t channels, uint32_t sample_rate, uint16_t bit_rate) {
    if (!self || !stream) {
        return NULL;
    }

    self->stream = stream;
    self->size = 0;
    self->format = format;
    self->channels = channels;
    self->sample_rate = sample_rate;
    self->channel_size = (bit_rate + 7) / 8;
    self->frame_size = channels * self->channel_size;
    self->frame_rate = sample_rate * self->frame_size;
    self->bit_rate = bit_rate;

    if (bat_wav_file_finish(self) != BAT_OK) {
        return NULL;
    }

    return self;
}


bat_error_t bat_wav_file_destroy(bat_wav_file_t **self) {
    if (!self || !*self) {
        return BAT_ERR;
    }
    free(*self);
    *self = NULL;
    return BAT_OK;
}


bat_error_t bat_wav_file_finish(bat_wav_file_t *self) {
    uint32_t total_size, chunk_size;
    uint16_t extra_param_size;
    if (!self) {
        return BAT_ERR;
    }

    if (fflush(self->stream)) {
        return BAT_ERR;
    }

    chunk_size = (
        sizeof(self->format)
        + sizeof(self->channels)
        + sizeof(self->sample_rate)
        + sizeof(self->frame_rate)
        + sizeof(self->frame_size)
        + sizeof(self->bit_rate)
    );

    if (self->format != BAT_WAV_PCM) {
        /* extra params size, must be included for non-pcm formats, unused */
        extra_param_size = 0;
        chunk_size += sizeof(extra_param_size);
    }

    /* file header + fmt chunk header + fmt chunk + data chunk header + data */
    total_size = 4 + 8 + chunk_size + 8 + self->size;
    /* the final file size is 8 bytes longer than the stated file size in the file itself, as that size is actually the chunk size of the top-level chunk */

    fseek(self->stream, 0, SEEK_SET);

    fputs("RIFF", self->stream);
    fwrite(&total_size, sizeof(total_size), 1, self->stream);
    fputs("WAVE", self->stream);
    fputs("fmt ", self->stream);
    fwrite(&chunk_size, sizeof(chunk_size), 1, self->stream);
    fwrite(&self->format, sizeof(self->format), 1, self->stream);
    fwrite(&self->channels, sizeof(self->channels), 1, self->stream);
    fwrite(&self->sample_rate, sizeof(self->sample_rate), 1, self->stream);
    fwrite(&self->frame_rate, sizeof(self->frame_rate), 1, self->stream);
    fwrite(&self->frame_size, sizeof(self->frame_size), 1, self->stream);
    fwrite(&self->bit_rate, sizeof(self->bit_rate), 1, self->stream);

    if (self->format != BAT_WAV_PCM) {
        /* extra params, unused */
        fwrite(&extra_param_size, sizeof(extra_param_size), 1, self->stream);
    }

    fputs("data", self->stream);
    fwrite(&self->size, sizeof(self->size), 1, self->stream);

    fflush(self->stream);

    return BAT_OK;
}


bat_error_t bat_wav_file_write(bat_wav_file_t *self, void *frame) {
    size_t size;
    if (!self) {
        return BAT_ERR;
    }

    size = 0;

    if (self->format == BAT_WAV_FLOAT) {
        size = fwrite(frame, self->channel_size, self->channels, self->stream);
        self->size += size * self->channel_size;
    } else if (self->format == BAT_WAV_PCM) {
        /* TODO: implement this*/
        return BAT_ERR;
    }

    if (!size) {
        return BAT_ERR;
    }

    return BAT_OK;
}
