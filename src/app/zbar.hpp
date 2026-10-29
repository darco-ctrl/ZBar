#pragma once

#include "app/bootstrap.hpp"
#include "app/paths.hpp"
#include "configs/config_manager.hpp"

class ZBar {
    public:
        ZBar()
            : config_manager(paths) {}
        
        void initialize();

    private:
        void create_bootstrap();
        void create_config_manager();
        
        Bootstrap bootstrap;
        ZBarPaths paths;
        ConfigManager config_manager;
};
