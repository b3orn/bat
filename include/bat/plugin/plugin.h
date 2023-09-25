#ifndef BAT_PLUGIN_H
#define BAT_PLUGIN_H

#define BAT_GET_PLUGIN(plugin) ((bat_plugin_t*)(plugin->plugin_data))


struct bat_plugin {
    bool active;
    bool processing;
    double sample_rate;
    uint32_t min_frames_count;
    uint32_t max_frames_count;
    clap_plugin_t plugin;
    const clap_host_t *host;
    bat_params_t *params;
    bat_state_t *state;
    bat_gui_t *gui;
};


bat_plugin_t* bat_plugin_new(const clap_plugin_descriptor_t *desc,
                             const clap_host_t *host);

void bat_plugin_free(bat_plugin_t *self);

bool CLAP_ABI bat_plugin_init(const clap_plugin_t *plugin);

void CLAP_ABI bat_plugin_destroy(const clap_plugin_t *plugin);

const void* CLAP_ABI bat_plugin_get_extension(const clap_plugin_t *plugin,
                                              const char *id);

bool CLAP_ABI bat_plugin_activate(const clap_plugin_t *plugin,
                                  double sample_rate,
                                  uint32_t min_frames_count,
                                  uint32_t max_frames_count);

void CLAP_ABI bat_plugin_deactivate(const clap_plugin_t *plugin);

bool CLAP_ABI bat_plugin_start_processing(const clap_plugin_t *plugin);

void CLAP_ABI bat_plugin_stop_processing(const clap_plugin_t *plugin);

void CLAP_ABI bat_plugin_reset(const clap_plugin_t *plugin);

clap_process_status CLAP_ABI bat_plugin_process(const struct clap_plugin *plugin,
                                                const clap_process_t *process);

void CLAP_ABI bat_plugin_on_main_thread(const struct clap_plugin *plugin);


#endif
