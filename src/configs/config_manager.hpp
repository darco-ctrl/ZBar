#pragma once

#include "app/paths.hpp"
#include "configs/config_parser.hpp"

class ConfigManager {
 public:
  explicit ConfigManager(ZBarPaths& paths) : config_parser(paths) {}

  void initialize();

  const BarsProperties& bars() const { return config_parser.bars(); }
  const ConfigValue& settings() const { return config_parser.settings(); }
  const BarsStyle& style() const { return config_parser.style(); }

 private:
  ConfigParser config_parser;
};
