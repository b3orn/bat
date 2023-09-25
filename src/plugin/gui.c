#include "bat.h"


static clap_plugin_gui_t bat_gui_api_descriptor = {
    .is_api_supported = bat_gui_is_api_supported,
    .get_preferred_api = NULL,
    .create = NULL,
    .destroy = NULL,
    .set_scale = NULL,
    .get_size = NULL,
    .can_resize = NULL,
    .get_resize_hints = NULL,
    .adjust_size = NULL,
    .set_size = NULL,
    .set_parent = NULL,
    .set_transient = NULL,
    .suggest_title = NULL,
    .show = NULL,
    .hide = NULL,
};


bat_gui_t* bat_gui_new(bat_plugin_t *plugin) {
    bat_gui_t *self;
    if (!plugin) {
        return NULL;
    }
    if (!(self = malloc(sizeof(*self)))) {
        return NULL;
    }

    self->plugin = plugin;
    self->api_descriptor = &bat_gui_api_descriptor;

    return self;
}


void bat_gui_free(bat_gui_t *self) {
    if (!self) {
        return;
    }
    free(self);
}


bool CLAP_ABI bat_gui_is_api_supported(const clap_plugin_t *plugin, const char *api, bool is_floating) {
    BAT_UNUSED(plugin);
    BAT_UNUSED(api);
    BAT_UNUSED(is_floating);
    return false;
}
