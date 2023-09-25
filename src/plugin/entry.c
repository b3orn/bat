#include "bat.h"


bool CLAP_ABI bat_entry_init(const char *plugin_path) {
    BAT_UNUSED(plugin_path);
    return true;
}


void CLAP_ABI bat_entry_deinit(void) {
    /* nothing here */
}


const void* CLAP_ABI bat_entry_get_factory(const char *factory_id) {
    if (strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID)) {
        return NULL;
    }
    return &bat_plugin_factory;
}


CLAP_EXPORT const clap_plugin_entry_t clap_entry = {
    .clap_version = CLAP_VERSION_INIT,
    .init = bat_entry_init,
    .deinit = bat_entry_deinit,
    .get_factory = bat_entry_get_factory,
};
