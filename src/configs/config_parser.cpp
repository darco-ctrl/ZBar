
#include <glaze/core/context.hpp>
#include <glaze/glaze.hpp>
#include <glaze/json/generic_fwd.hpp>
#include <glaze/json/read.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "configs/config_parser.hpp"
#include "configs/types/bar.hpp"
#include "core/types.hpp"

void ConfigParser::initialize() {

}

BarsProperties ConfigParser::get_bars_properties() {
    BarsProperties bars_properties;
    
    std::ifstream file(paths_.bars_jsonc);
    std::stringstream stream;
    stream << file.rdbuf();

    std::string buffer = stream.str();

    glz::error_ctx error = glz::read_file_jsonc(
        bars_properties,
        paths_.bars_jsonc.string(),
        buffer
    );

    return bars_properties;
}
