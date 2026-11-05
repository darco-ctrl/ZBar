#include "configs/config_parser.hpp"

#include <fstream>
#include <glaze/glaze.hpp>
#include <glaze/json/read.hpp>
#include <glaze/toml/read.hpp>
#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace {
using Generic = glz::generic_i64;

// Read an entire config file into a string for Glaze to parse.
std::string read_file(const zbar::Path& path) {
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    std::cerr << "Could not open config file: " << path.string() << '\n';
    return {};
  }
  std::ostringstream stream;
  stream << file.rdbuf();
  return stream.str();
}

// Look up a key only when the generic value is a JSON/TOML object.
const Generic* find(const Generic& value, std::string_view key) {
  const auto* object = value.get_if<Generic::object_t>();
  if (!object) return nullptr;
  const auto found = object->find(key);
  return found == object->end() ? nullptr : &found->second;
}

// Return a string value when present; otherwise use the supplied default.
std::string string_or(const Generic* value, std::string fallback = {}) {
  if (value) {
    if (const auto* string = value->get_if<std::string>()) return *string;
  }
  return fallback;
}

// Copy only string elements from a generic array into a compact string vector.
void read_string_list(const Generic* value, std::vector<std::string>& output) {
  if (!value) return;
  const auto* array = value->get_if<Generic::array_t>();
  if (!array) return;
  output.reserve(array->size());
  for (const auto& item : *array) {
    if (const auto* string = item.get_if<std::string>()) output.push_back(*string);
  }
}

// Replace strings such as "$foreground" with values from [variables],
// recursively handling nested tables and arrays before styles are converted.
void resolve_variables(Generic& value, const Generic& variables, unsigned depth = 0) {
  if (depth > 16) return;  // Prevent accidental variable-reference cycles.

  if (auto* string = value.get_if<std::string>()) {
    if (string->size() > 1 && string->front() == '$') {
      if (const Generic* replacement = find(variables, std::string_view(*string).substr(1))) {
        value = *replacement;
        resolve_variables(value, variables, depth + 1);
      }
    }
    return;
  }

  if (auto* object = value.get_if<Generic::object_t>()) {
    for (auto& [key, child] : *object) {
      (void)key;
      resolve_variables(child, variables, depth + 1);
    }
  } else if (auto* array = value.get_if<Generic::array_t>()) {
    for (auto& child : *array) resolve_variables(child, variables, depth + 1);
  }
}

// Read an optional TOML string field without coercing values of other types.
std::optional<std::string> read_optional_string(const Generic* value) {
  if (value) {
    if (const auto* string = value->get_if<std::string>()) return *string;
  }
  return std::nullopt;
}

// Convert the standard style fields to typed storage and retain custom fields.
StyleProperties read_style_properties(const Generic& value) {
  StyleProperties result;
  result.background_color = read_optional_string(find(value, "background_color"));
  result.color = read_optional_string(find(value, "color"));

  const Generic* radius_value = find(value, "radius");
  if (radius_value) {
    if (const auto* radius = radius_value->get_if<Generic::array_t>();
        radius && radius->size() == 4) {
      std::array<int8_t, 4> corners{};
      bool valid = true;
      for (size_t i = 0; i < corners.size(); ++i) {
        const auto* number = (*radius)[i].get_if<int64_t>();
        if (!number || *number < std::numeric_limits<int8_t>::min() ||
            *number > std::numeric_limits<int8_t>::max()) {
          valid = false;
          break;
        }
        corners[i] = static_cast<int8_t>(*number);
      }
      if (valid) result.radius = corners;
    }
  }

  if (const auto* properties = value.get_if<Generic::object_t>()) {
    for (const auto& [name, property] : *properties) {
      if (name == "background_color" || name == "color" || name == "radius") continue;
      result.extra.push_back({name, property});
    }
  }
  return result;
}

// Parse one interaction state, such as idle, hover, or active, if it exists.
std::optional<StyleProperties> read_style_state(const Generic& table,
                                                std::string_view state_name) {
  const Generic* state = find(table, state_name);
  if (!state) return std::nullopt;
  return read_style_properties(*state);
}

// Gather the supported interaction states from one bar or widget table.
Style parse_style(const Generic& table) {
  Style style;
  style.idle = read_style_state(table, "idle");
  style.hover = read_style_state(table, "hover");
  style.active = read_style_state(table, "active");
  return style;
}
}  // namespace

// Load all three config files into this parser's stored config structures.
void ConfigParser::initialize() {
  read_bars();
  read_settings();
  read_style();
}

// Parse each top-level JSONC entry as a bar and extract its layout and widgets.
void ConfigParser::read_bars() {
  Generic root;
  const std::string buffer = read_file(paths_.bars_jsonc);
  const glz::error_ctx error = glz::read_jsonc(root, buffer);
  if (error) {
    std::cerr << glz::format_error(error, buffer) << '\n';
    return;
  }

  auto* object = root.get_if<Generic::object_t>();
  if (!object) {
    std::cerr << "bars.jsonc must contain an object at its root.\n";
    return;
  }

  BarsProperties parsed;
  parsed.bars.reserve(object->size());
  for (auto& [name, value] : *object) {
    BarProperties bar;
    bar.name = name;
    bar.position = string_or(find(value, "position"), "top");
    bar.monitor = string_or(find(value, "monitor"), "primary");
    read_string_list(find(value, "widgets-left"), bar.widgets_left);
    read_string_list(find(value, "widgets-center"), bar.widgets_center);
    read_string_list(find(value, "widgets-right"), bar.widgets_right);

    if (const Generic* widgets_value = find(value, "widgets")) {
      if (const auto* widgets = widgets_value->get_if<Generic::object_t>()) {
        bar.widgets.reserve(widgets->size());
        for (auto& [widget_name, properties] : *widgets) {
          bar.widgets.push_back({widget_name, std::move(properties)});
        }
      }
    }
    parsed.bars.push_back(std::move(bar));
  }
  bars_ = std::move(parsed);
}

// Keep the settings document as a generic tree so all JSON value types survive.
void ConfigParser::read_settings() {
  Generic parsed;
  const std::string buffer = read_file(paths_.settings_jsonc);
  const glz::error_ctx error = glz::read_jsonc(parsed, buffer);
  if (error) {
    std::cerr << glz::format_error(error, buffer) << '\n';
    return;
  }
  settings_ = std::move(parsed);
}

// Resolve TOML variables, then convert bar and widget tables to compact structs.
void ConfigParser::read_style() {
  std::unordered_map<std::string, Generic> sections;
  const std::string buffer = read_file(paths_.style_toml);
  const glz::error_ctx error = glz::read_toml(sections, buffer);
  if (error) {
    std::cerr << glz::format_error(error, buffer) << '\n';
    return;
  }

  Generic parsed;
  parsed.data = Generic::object_t{};
  auto* parsed_root = parsed.get_if<Generic::object_t>();
  for (auto& [name, section] : sections) {
    parsed_root->emplace(name, std::move(section));
  }

  auto* root = parsed.get_if<Generic::object_t>();
  const Generic* variables = find(parsed, "variables");
  if (variables && root) {
    for (auto& [name, value] : *root) {
      if (name != "variables") resolve_variables(value, *variables);
    }
    root->erase("variables");
  }

  BarsStyle result;
  if (root) {
    result.bars.reserve(root->size());
    for (const auto& [bar_name, table] : *root) {
      BarStyle bar;
      bar.name = bar_name;
      bar.style = parse_style(table);

      if (const auto* entries = table.get_if<Generic::object_t>()) {
        bar.widgets.reserve(entries->size());
        for (const auto& [widget_name, widget_table] : *entries) {
          if (widget_name == "idle" || widget_name == "hover" ||
              widget_name == "active")
            continue;
          if (widget_table.get_if<Generic::object_t>()) {
            bar.widgets.push_back({widget_name, parse_style(widget_table)});
          } else {
            bar.extra.push_back({widget_name, widget_table});
          }
        }
      }
      result.bars.push_back(std::move(bar));
    }
  }
  style_ = std::move(result);
}
