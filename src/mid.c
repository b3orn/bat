#include "bat.h"


static uint16_t bat_mid_flip16(uint16_t value) {
    return (uint16_t)((value & 0x00ff) << 8) | ((value & 0xff00) >> 8);
}


static uint32_t bat_mid_flip32(uint32_t value) {
    return (uint32_t)(
        ((value & 0x000000ff) << 24)
        | ((value & 0x0000ff00) << 8)
        | ((value & 0x00ff0000) >> 8)
        | ((value & 0xff000000) >> 24)
    );
}


static bat_error_t bat_mid_file_read_header(bat_mid_file_t *self) {
    char buf[10];
    uint32_t size;
    uint16_t format, n, division;
    if (!self || ferror(self->stream) || feof(self->stream)) {
        return BAT_ERR;
    }

    memset(buf, 0, 10);

    if (!fread(buf, 1, 4, self->stream) || strncmp(buf, "MThd", 4)) {
        return BAT_ERR;
    }

    fread(&size, sizeof(size), 1, self->stream);
    if (bat_mid_flip32(size) != 6) {
        return BAT_ERR;
    }

    fread(&format, sizeof(format), 1, self->stream);
    if (bat_mid_flip16(format) != 0) {
        /* only supports format 0, single track */
        return BAT_ERR;
    }

    fread(&n, sizeof(n), 1, self->stream);
    if (bat_mid_flip16(n) != 1) {
        /* shouldn't actually happen, but who knows, format zero only has one track */
        return BAT_ERR;
    }

    fread(&division, sizeof(division), 1, self->stream);
    if (division & 0x8000) {
        /* only supporting ticks per quarter-note for now */
        return BAT_ERR;
    }

    self->ticks = bat_mid_flip16(division);

    /* read track header */

    if (!fread(buf, 1, 4, self->stream) || strncmp(buf, "MTrk", 4)) {
        return BAT_ERR;
    }

    fread(&size, sizeof(size), 1, self->stream);
    self->size = (int64_t)bat_mid_flip32(size);

    return BAT_OK;
}


bat_mid_file_t* bat_mid_file_new(FILE *stream) {
    bat_mid_file_t *self;
    if (!stream || !(self = malloc(sizeof(*self)))) {
        return NULL;
    }

    if (!bat_mid_file_init(self, stream)) {
        bat_mid_file_destroy(&self);
        return NULL;
    }

    return self;
}


bat_mid_file_t* bat_mid_file_init(bat_mid_file_t *self, FILE *stream) {
    if (!self || !stream) {
        return NULL;
    }
    self->stream = stream;
    self->ticks = 0;
    self->size = 0;

    if (bat_mid_file_read_header(self) != BAT_OK) {
        return NULL;
    }

    return self;
}


bat_error_t bat_mid_file_destroy(bat_mid_file_t **self) {
    if (!self || !*self) {
        return BAT_ERR;
    }
    free(*self);
    *self = NULL;
    return BAT_OK;
}


static int64_t bat_mid_file_read_vlq(bat_mid_file_t *self) {
    int64_t r;
    uint8_t c;
    if (!self || self->size <= 0) {
        return -1;
    }

    r = 0;
    do {
        r <<= 7;
        c = (uint8_t)fgetc(self->stream);
        r |= c & 0x7f;
        --self->size;
    } while(c & 0x80);

    return r;
}


bat_error_t bat_mid_file_next_event(bat_mid_file_t *self, bat_inst_t *inst, bat_mid_event_t *event) {
    int64_t dt, size;
    uint8_t status, type, a, b;
    double c;
    if (!self || !event || self->size <= 0) {
        return BAT_ERR;
    }

    do {
        if ((dt = bat_mid_file_read_vlq(self)) < 0) {
            return BAT_ERR;
        }

        status = (uint8_t)fgetc(self->stream);
        --self->size;

        if ((status & 0xff) == 0xff) {
            /* meta event, let's just skip these */
            type = (uint8_t)fgetc(self->stream);
            --self->size;
            if (type == 0x2f) {
                /* end of track */
                event->type = BAT_END_OF_TRACK_EVENT;
                return BAT_OK;
            } else if (type == 0x58) {
                /* time signature */
                fseek(self->stream, 1, SEEK_CUR);
                a = (uint8_t)fgetc(self->stream);
                b = (uint8_t)fgetc(self->stream);
                fseek(self->stream, 2, SEEK_CUR);
                c = pow(2, b);
                /* don't care about the other two paramters,
                   32nd notes in a 4th notes, if this isn't 8 then wtf */
                self->size -= 4;
                inst->sig_num = a;
                inst->sig_denom = (uint8_t)c;
            } else if (type == 0) {
                fseek(self->stream, 1, SEEK_CUR);
                ++self->size;
            } else if (type <= 7) {
                if ((size = bat_mid_file_read_vlq(self)) < 0) {
                    return BAT_ERR;
                }
                self->size -= size;
                fseek(self->stream, size, SEEK_CUR);
            }
            /* there's more, let's hope this is enough for now */
        } else if ((status & 0xf0) == 0xf0) {
            /* sysex event, skip these too */
            self->size -= 1 + fgetc(self->stream);
        } else {
            /* midi event */
            event->type = BAT_MIDI_EVENT;
            event->dt = (uint64_t)dt;
            event->status = status;
            event->data0 = (uint8_t)fgetc(self->stream);
            event->data1 = (uint8_t)fgetc(self->stream);
            self->size -= 2;
            return BAT_OK;
        }
    } while (self->size > 0);

    /* this point should be unreachable as there should be an end of track meta event occuring */

    return BAT_ERR;
}
