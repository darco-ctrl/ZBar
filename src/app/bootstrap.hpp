#pragma once

#include "app/paths.hpp"
#include "core/types.hpp"

class Bootstrap {
    public:
        void initialize();
        ZBarPaths create_zbar_paths();
        
    private:
        zbar::path get_zbar_directory();
        zbar::path get_home_directory();
        
        void check_paths();
};
