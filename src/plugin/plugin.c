#include "bat.h"


bat_plugin_t* bat_plugin_new(const clap_plugin_descriptor_t *desc, const clap_host_t *host) {
    bat_plugin_t *self;
    bat_params_t *params;
    bat_state_t *state;
    bat_gui_t *gui;
    if (!desc || !host) {
        return NULL;
    }
    if (!(self = malloc(sizeof(*self)))) {
        return NULL;
    }
    memset(self, 0, sizeof(*self));

    if (!(params = bat_params_new(self))) {
        goto error;
    }
    if (!(state = bat_state_new(self))) {
        goto error;
    }
    if (!(gui = bat_gui_new(self))) {
        goto error;
    }

    self->active = false;
    self->processing = false;
    self->host = host;
    self->params = params;
    self->state = state;
    self->gui = gui;

    self->plugin.plugin_data = self;

    self->plugin.desc = desc;
    self->plugin.init = bat_plugin_init;
    self->plugin.destroy = bat_plugin_destroy;
    self->plugin.activate = bat_plugin_activate;
    self->plugin.deactivate = bat_plugin_deactivate;
    self->plugin.start_processing = bat_plugin_start_processing;
    self->plugin.stop_processing = bat_plugin_stop_processing;
    self->plugin.reset = bat_plugin_reset;
    self->plugin.process = bat_plugin_process;
    self->plugin.get_extension = bat_plugin_get_extension;
    self->plugin.on_main_thread = bat_plugin_on_main_thread;

    return self;

error:
    bat_plugin_free(self);

    return NULL;
}


void bat_plugin_free(bat_plugin_t *self) {
    if (!self) {
        return;
    }
    bat_params_free(self->params);
    bat_state_free(self->state);
    bat_gui_free(self->gui);
    free(self);
}


bool CLAP_ABI bat_plugin_init(const clap_plugin_t *plugin) {
    if (!plugin) {
        return false;
    }
    return true;
}


void CLAP_ABI bat_plugin_destroy(const clap_plugin_t *plugin) {
    if (!plugin) {
        return;
    }
    bat_plugin_free(plugin->plugin_data);
}


const void* CLAP_ABI bat_plugin_get_extension(const clap_plugin_t *plugin,
                                              const char *id) {
    bat_plugin_t *self;
    if (!plugin || !plugin->plugin_data) {
        return NULL;
    }

    self = BAT_GET_PLUGIN(plugin);
    if (!strcmp(id, CLAP_EXT_PARAMS) && self->params) {
        return self->params->api_descriptor;
    } else if (!strcmp(id, CLAP_EXT_STATE) && self->state) {
        return self->state->api_descriptor;
    } else if (!strcmp(id, CLAP_EXT_GUI) && self->gui) {
        return self->gui->api_descriptor;
    }

    return NULL;
}


bool CLAP_ABI bat_plugin_activate(const clap_plugin_t *plugin,
                                  double sample_rate,
                                  uint32_t min_frames_count,
                                  uint32_t max_frames_count) {
    bat_plugin_t *self;
    if (!plugin) {
        return false;
    }
    
    self = BAT_GET_PLUGIN(plugin);
    self->active = true;
    self->sample_rate = sample_rate;
    self->min_frames_count = min_frames_count;
    self->max_frames_count = max_frames_count;

    return true;
}


void CLAP_ABI bat_plugin_deactivate(const clap_plugin_t *plugin) {
    bat_plugin_t *self;
    if (!plugin) {
        return;
    }
    
    self = BAT_GET_PLUGIN(plugin);
    self->active = false;
}


bool CLAP_ABI bat_plugin_start_processing(const clap_plugin_t *plugin) {
    bat_plugin_t *self;
    if (!plugin) {
        return false;
    }
    
    self = BAT_GET_PLUGIN(plugin);
    self->processing = true;

    return true;
}


void CLAP_ABI bat_plugin_stop_processing(const clap_plugin_t *plugin) {
    bat_plugin_t *self;
    if (!plugin) {
        return;
    }
    
    self = BAT_GET_PLUGIN(plugin);
    self->processing = false;
}


void CLAP_ABI bat_plugin_reset(const clap_plugin_t *plugin) {
    bat_plugin_t *self;
    if (!plugin) {
        return;
    }
    
    self = BAT_GET_PLUGIN(plugin);
    BAT_UNUSED(self);
}


clap_process_status CLAP_ABI bat_plugin_process(const struct clap_plugin *plugin,
                                                const clap_process_t *process) {
    bat_plugin_t *self;
    if (!plugin || !process) {
        return CLAP_PROCESS_ERROR;
    }

    self = BAT_GET_PLUGIN(plugin);
    BAT_UNUSED(self);

    return CLAP_PROCESS_ERROR;
}


void CLAP_ABI bat_plugin_on_main_thread(const struct clap_plugin *plugin) {
    bat_plugin_t *self;
    if (!plugin) {
        return;
    }

    self = BAT_GET_PLUGIN(plugin);
    BAT_UNUSED(self);
}
