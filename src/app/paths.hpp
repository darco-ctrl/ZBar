#pragma once

#include <filesystem>

struct AppPaths {
    std::filesystem::path config_directory;
    std::filesystem::path bars_jsonc;
    std::filesystem::path settings_jsonc;
    std::filesystem::path style_toml;
};
