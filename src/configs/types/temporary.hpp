#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>

using ConfigVariant = std::variant<
    std::string, 
    std::int8_t, 
    std::array<int8_t, 4>,
    bool
>;
using Variables = std::unordered_map<std::string, ConfigVariant>;
