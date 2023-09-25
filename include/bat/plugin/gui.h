#ifndef BAT_PLUGIN_GUI_H
#define BAT_PLUGIN_GUI_H

struct bat_gui {
    bat_plugin_t *plugin;
    clap_plugin_gui_t *api_descriptor;
};


bat_gui_t* bat_gui_new(bat_plugin_t *plugin);

void bat_gui_free(bat_gui_t *self);

bool CLAP_ABI bat_gui_is_api_supported(const clap_plugin_t *plugin, const char *api, bool is_floating);


#endif
