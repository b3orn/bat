#ifndef BAT_PLUGIN_PARAMS_H
#define BAT_PLUGIN_PARAMS_H

struct bat_params {
    bat_plugin_t *plugin;
    clap_plugin_params_t *api_descriptor;
    uint32_t count;
};


bat_params_t* bat_params_new(bat_plugin_t *plugin);

void bat_params_free(bat_params_t *self);

uint32_t CLAP_ABI bat_params_count(const clap_plugin_t *plugin);


#endif
