#pragma once

#include "app/paths.hpp"
#include "configs/types/bar.hpp"
#include "configs/types/style.hpp"

class ConfigParser {
 public:
  explicit ConfigParser(ZBarPaths& paths) : paths_(paths) {}

  void initialize();

  const BarsProperties& bars() const { return bars_; }
  const ConfigValue& settings() const { return settings_; }
  const BarsStyle& style() const { return style_; }

 private:
  ZBarPaths& paths_;
  BarsProperties bars_;
  ConfigValue settings_;
  BarsStyle style_;

  void read_bars();
  void read_settings();
  void read_style();
};
