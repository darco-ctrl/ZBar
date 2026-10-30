#pragma once

#include "configs/types/style.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>


namespace zbar {
    using Path = std::filesystem::path;
    
    using BarStyleMap = std::unordered_map<std::string, BarStyle>;
}
