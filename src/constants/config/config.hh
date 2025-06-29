#ifndef CONFIG_HH
#define CONFIG_HH

#include <QString>

#include <3rdparty/toml/toml.hh>

#include <iostream>

namespace Config {

extern toml::table CONFIGS;

void initialize();

template <typename T> T retrieve(const toml::node_view<toml::node> &node) {
    try {
        return node.value<T>().value();
    } catch (const std::bad_optional_access &err) {
        std::cout << "An error occurred while trying to retrieve a configuration value.\n"
                     "Please use a debugger to find out where it happened and verify if the keys are correctly "
                     "defined in the TOML file.\n\n";
        throw err;
    }
}

template <> QString retrieve(const toml::node_view<toml::node> &node);

} // namespace Config

#endif