#pragma once

#include "app/paths.hpp"
#include "configs/config_parser.hpp"
#include "configs/types/bar.hpp"
#include "core/types.hpp"

class ConfigManager {
    public:
        explicit ConfigManager(ZBarPaths& paths)
             :  paths_(paths),
                config_parser(paths_) {}

        void initialize();

    private:
        ZBarPaths& paths_;

        ConfigParser config_parser;
        BarsProperties bars_properties;
        zbar::BarStyleMap bars_style;

        BarsProperties create_bars_config();
        zbar::BarStyleMap create_bars_style();
};
