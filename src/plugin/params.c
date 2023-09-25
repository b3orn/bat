#include "bat.h"


static clap_plugin_params_t bat_params_api_descriptor = {
    .count = bat_params_count,
    .get_info = NULL,
    .get_value = NULL,
    .value_to_text = NULL,
    .text_to_value = NULL,
    .flush = NULL,
};


bat_params_t* bat_params_new(bat_plugin_t *plugin) {
    bat_params_t *self;
    if (!plugin) {
        return NULL;
    }
    if (!(self = malloc(sizeof(*self)))) {
        return NULL;
    }

    self->plugin = plugin;
    self->api_descriptor = &bat_params_api_descriptor;
    self->count = 0;

    return self;
}


void bat_params_free(bat_params_t *self) {
    if (!self) {
        return;
    }
    free(self);
}


uint32_t CLAP_ABI bat_params_count(const clap_plugin_t *plugin) {
    bat_plugin_t *self;
    if (!plugin || !plugin->plugin_data) {
        return 0;
    }

    self = BAT_GET_PLUGIN(plugin);

    return self->params->count;
}
