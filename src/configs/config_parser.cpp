
#include <glaze/core/context.hpp>
#include <glaze/glaze.hpp>
#include <glaze/json/generic_fwd.hpp>
#include <glaze/json/read.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "configs/config_parser.hpp"

void ConfigParser::initialize() {
    create_bars_jsonc_map();
    std::cout << bars_config_map["primary_bar"]["position"].get<std::string>() << std::endl;;
}

void ConfigParser::create_bars_jsonc_map() {
    std::ifstream file(paths_.bars_jsonc);
    std::stringstream stream;
    stream << file.rdbuf();

    std::string buffer = stream.str();

    glz::error_ctx error = glz::read_file_jsonc(
        bars_config_map,
        paths_.bars_jsonc.string(),
        buffer
    );
}
