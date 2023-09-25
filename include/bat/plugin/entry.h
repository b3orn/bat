#ifndef BAT_PLUGIN_ENTRY_H
#define BAT_PLUGIN_ENTRY_H


bool CLAP_ABI bat_entry_init(const char *plugin_path);

void CLAP_ABI bat_entry_deinit(void);

const void* CLAP_ABI bat_entry_get_factory(const char *factory_id);


#endif
