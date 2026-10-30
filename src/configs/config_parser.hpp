#pragma once

#include "app/paths.hpp"
#include "configs/types/bar.hpp"
#include "configs/types/temporary.hpp"
#include <glaze/json/generic_fwd.hpp>

class ConfigParser {
    public: 
        explicit ConfigParser(ZBarPaths& paths)
            : paths_(paths) {}
        
        void initialize();
        BarsProperties get_bars_properties();

    private:
        ZBarPaths& paths_;
         
        Variables style_variables;
};
