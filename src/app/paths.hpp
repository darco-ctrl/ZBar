#pragma once

#include "core/types.hpp"

struct ZBarPaths {
    zbar::Path config_directory;
    zbar::Path bars_jsonc;
    zbar::Path settings_jsonc;
    zbar::Path style_toml;
};
