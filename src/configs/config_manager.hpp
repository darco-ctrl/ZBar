#pragma once

#include "app/paths.hpp"
#include "configs/types/bar.hpp"
#include "configs/types/style.hpp"
#include <unordered_map>
class ConfigManager {
    public:
        explicit ConfigManager(ZBarPaths& paths)
             : paths_(paths) {}
        
        void initialize();

    private:
        ZBarPaths& paths_;
        
        std::unordered_map<std::string, BarConfig> bars_config;
        std::unordered_map<std::string, BarStyle> bars_style;

        void set_paths(ZBarPaths& paths);
        void create_bars_config();
        void create_bars_style();
};
