#pragma once

#include <array>
#include <optional>
#include <cstdint>
#include <string>
#include <unordered_map>

struct StyleProperties {
    std::optional<std::string> background_color;
    std::optional<std::string> color;
    std::optional<std::array<int8_t, 4>> radius;
};

struct Style {
    std::optional<StyleProperties> idle;
    std::optional<StyleProperties> hover;
    std::optional<StyleProperties> active;
};

using StyleMap = std::unordered_map<std::string, StyleProperties>;

struct BarStyle {
    std::string id;
    Style style; 
    
    StyleMap styles;
};
