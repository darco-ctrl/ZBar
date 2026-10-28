#pragma once

#include <array>
#include <optional>
#include <cstdint>
#include <string>
#include <unordered_map>

struct Style {
    std::optional<std::string> background_color;
    std::optional<std::string> color;
    std::optional<std::array<uint8_t, 4>> radius;
};

using StyleMap = std::unordered_map<std::string, Style>;

struct BarStyle {
    std::string id;
    StyleMap styles;
};
