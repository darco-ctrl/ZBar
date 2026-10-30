#pragma once

#include "app/paths.hpp"
#include "configs/types/temporary.hpp"
#include <glaze/json/generic_fwd.hpp>

class ConfigParser {
    public: 
        explicit ConfigParser(ZBarPaths& paths)
            : paths_(paths) {}
        
        void initialize();
        void create_bars_jsonc_map();

    private:
        ZBarPaths& paths_;
        
        glz::generic bars_config_map; 
        Variables style_variables;
};
