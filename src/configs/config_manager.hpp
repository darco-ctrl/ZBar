#pragma once

#include "configs/types/bar.hpp"
#include "configs/types/style.hpp"
#include <unordered_map>
class ConfigManager {
    public: 
        void initialize();

    private:        
        std::unordered_map<std::string, BarConfig> bars_config;
        std::unordered_map<std::string, BarStyle> bars_style;

        void create_bars_config();
        void create_bars_style();
};
