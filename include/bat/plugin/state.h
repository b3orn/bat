#ifndef BAT_PLUGIN_STATE_H
#define BAT_PLUGIN_STATE_H

struct bat_state {
    bat_plugin_t *plugin;
    clap_plugin_state_t *api_descriptor;
};


bat_state_t* bat_state_new(bat_plugin_t *plugin);

void bat_state_free(bat_state_t *self);

bool CLAP_ABI bat_state_save(const clap_plugin_t *plugin, const clap_ostream_t *stream);

bool CLAP_ABI bat_state_load(const clap_plugin_t *plugin, const clap_istream_t *stream);


#endif
