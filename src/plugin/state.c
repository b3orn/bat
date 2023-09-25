#include "bat.h"


static clap_plugin_state_t bat_state_api_descriptor = {
    .save = bat_state_save,
    .load = bat_state_load,
};


bat_state_t* bat_state_new(bat_plugin_t *plugin) {
    bat_state_t *self;
    if (!plugin) {
        return NULL;
    }
    if (!(self = malloc(sizeof(*self)))) {
        return NULL;
    }

    self->plugin = plugin;
    self->api_descriptor = &bat_state_api_descriptor;

    return self;
}


void bat_state_free(bat_state_t *self) {
    if (!self) {
        return;
    }
    free(self);
}


bool CLAP_ABI bat_state_save(const clap_plugin_t *plugin, const clap_ostream_t *stream) {
    bat_plugin_t *self;
    if (!plugin || !plugin->plugin_data) {
        return false;
    }

    self = (bat_plugin_t*)plugin->plugin_data;
    BAT_UNUSED(self);
    BAT_UNUSED(stream);
    return false;
}


bool CLAP_ABI bat_state_load(const clap_plugin_t *plugin, const clap_istream_t *stream) {
    bat_plugin_t *self;
    if (!plugin || !plugin->plugin_data) {
        return false;
    }

    self = BAT_GET_PLUGIN(plugin);
    BAT_UNUSED(self);
    BAT_UNUSED(stream);
    return false;
}
