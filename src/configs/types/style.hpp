#pragma once

#include "configs/types/bar.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct ExtraStyleProperty {
  std::string name;
  ConfigValue value;
};

struct StyleProperties {
  std::optional<std::string> background_color;
  std::optional<std::string> color;
  std::optional<std::array<int8_t, 4>> radius;
  std::vector<ExtraStyleProperty> extra;
};

struct Style {
  std::optional<StyleProperties> idle;
  std::optional<StyleProperties> hover;
  std::optional<StyleProperties> active;
};

struct WidgetStyle {
  std::string name;
  Style style;
};

struct BarStyle {
  std::string name;
  Style style;
  std::vector<WidgetStyle> widgets;
  std::vector<ExtraStyleProperty> extra;
};

struct BarsStyle {
  std::vector<BarStyle> bars;
};
