#pragma once

#include <string>
#include <unordered_map>

struct BarProperties {
    std::string id;
    std::string position = "top";
    std::string monitor = "primary";
};

struct BarsProperties {
    std::unordered_map<std::string, BarProperties> bars;
};
