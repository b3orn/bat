#ifndef BAT_PLUGIN_FACTORY_H
#define BAT_PLUGIN_FACTORY_H

extern const clap_plugin_factory_t bat_plugin_factory;


uint32_t CLAP_ABI bat_factory_get_plugin_count(const clap_plugin_factory_t *factory);

const clap_plugin_descriptor_t* CLAP_ABI bat_factory_get_plugin_descriptor(const clap_plugin_factory_t *factory, uint32_t index);

const clap_plugin_t* CLAP_ABI bat_factory_create_plugin(const clap_plugin_factory_t *factory, const clap_host_t *host, const char *plugin_id);


#endif
