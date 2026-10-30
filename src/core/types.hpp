#pragma once

#include "configs/types/bar.hpp"
#include "configs/types/style.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>


namespace zbar {
    using Path = std::filesystem::path;
    
    using BarConfigMap = std::unordered_map<std::string, BarConfig>;
    using BarStyleMap = std::unordered_map<std::string, BarStyle>;
}
