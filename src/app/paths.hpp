#pragma once

#include "core/types.hpp"

struct ZBarPaths {
    zbar::path config_directory;
    zbar::path bars_jsonc;
    zbar::path settings_jsonc;
    zbar::path style_toml;
};
