#pragma once

#include "app/paths.hpp"

class Bootstrap {
    public:
        void initialize();
        
    private:
        AppPaths paths;

        std::filesystem::path get_zbar_directory();
        std::filesystem::path get_home_directory();
        void create_app_paths();
        void check_paths();
};
