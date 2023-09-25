#include "bat.h"


static const clap_plugin_descriptor_t bat_plugin_descriptor = {
    .clap_version = CLAP_VERSION_INIT,
    .id = "org.fac3.bat",
    .name = "BAT",
    .vendor = "fac3",
    .version = BAT_VERSION,
    .description = "BAT Synthesizer",
    .url = "https://fac3.org/bat",
    .manual_url = "https://fac3.org/bat",
    .support_url = "https://fac3.org",
    .features = (const char*[]){
        CLAP_PLUGIN_FEATURE_INSTRUMENT,
        CLAP_PLUGIN_FEATURE_SYNTHESIZER,
        CLAP_PLUGIN_FEATURE_STEREO,
        NULL,
    },
};


uint32_t CLAP_ABI bat_factory_get_plugin_count(const clap_plugin_factory_t *factory) {
    BAT_UNUSED(factory);
    return 1;
}

const clap_plugin_descriptor_t* CLAP_ABI bat_factory_get_plugin_descriptor(const clap_plugin_factory_t *factory, uint32_t index) {
    BAT_UNUSED(factory);
    BAT_UNUSED(index);
    return &bat_plugin_descriptor;
}


const clap_plugin_t* CLAP_ABI bat_factory_create_plugin(const clap_plugin_factory_t *factory, const clap_host_t *host, const char *plugin_id) {
    bat_plugin_t *plugin;
    BAT_UNUSED(factory);
    if (!host || !plugin_id) {
        return NULL;
    }
    if (!clap_version_is_compatible(host->clap_version) || strcmp(plugin_id, bat_plugin_descriptor.id)) {
        return NULL;
    }
    if (!(plugin = bat_plugin_new(&bat_plugin_descriptor, host))) {
        return NULL;
    }
    return &plugin->plugin;
}


const clap_plugin_factory_t bat_plugin_factory = {
    .get_plugin_count = bat_factory_get_plugin_count,
    .get_plugin_descriptor = bat_factory_get_plugin_descriptor,
    .create_plugin = bat_factory_create_plugin,
};
