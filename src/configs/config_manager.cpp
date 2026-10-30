

#include "configs/config_manager.hpp"

void ConfigManager::initialize() {
    config_parser.initialize();

    bars_properties = config_parser.get_bars_properties();
}
