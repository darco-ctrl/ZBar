#pragma once

#include <glaze/json/generic.hpp>
#include <string>
#include <vector>

using ConfigValue = glz::generic_i64;

struct WidgetProperties {
  std::string name;
  ConfigValue values;
};

struct BarProperties {
  std::string name;
  std::string position;
  std::string monitor;
  std::vector<std::string> widgets_left;
  std::vector<std::string> widgets_center;
  std::vector<std::string> widgets_right;
  std::vector<WidgetProperties> widgets;
};

struct BarsProperties {
  std::vector<BarProperties> bars;
};
