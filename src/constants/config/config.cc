#include <cstdlib>

#include "3rdparty/toml/toml.hh"

#include "config.hh"
#include <iostream>

namespace Config {

toml::table CONFIGS;

void initialize() {
    const char *configFilepath = std::getenv("WRTACTIX_CONFIG_FILEPATH");
    if (configFilepath == nullptr) {
        configFilepath = DEFAULT_CONFIG_FILEPATH;
    }

    std::cout << "[INFO] Reading config file [" << configFilepath << "].\n";
    CONFIGS = toml::parse_file(configFilepath);
}

template <> QString retrieve(const toml::node_view<toml::node> &node) {
    auto value = retrieve<const char *>(node);
    return {value};
}

} // namespace Config