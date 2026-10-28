
#include <iostream>
#include <libloaderapi.h>
#include <minwindef.h>
#include <windows.h>
#include <shlobj.h>
#include <filesystem>
// #include <string>
#include <stdexcept>

#include "app/bootstrap.hpp"
#include "core/types.hpp"
// #include "app/paths.hpp"
#include <filesystem>
#include <winnt.h>

void Bootstrap::initialize() {
    create_app_paths();
}

std::filesystem::path Bootstrap::get_zbar_directory() {
    wchar_t buffer[MAX_PATH];

    GetModuleFileNameW(nullptr, buffer, MAX_PATH);

    return std::filesystem::path(buffer).parent_path();
}

std::filesystem::path Bootstrap::get_home_directory() {
    PWSTR path = nullptr;

    HRESULT result = SHGetKnownFolderPath(
        FOLDERID_Profile,
        0,
        nullptr,
        &path
    );

    if (FAILED(result)) {
        throw std::runtime_error(
            "Failed to get user home directory."
        );
    }

    std::filesystem::path home(path);
    CoTaskMemFree(path);

    return home;
}


void Bootstrap::create_app_paths() {
    zbar::Path home_directory = get_home_directory();

    zbar::Path default_configs_directory = get_zbar_directory() / "configs" / "defaults";
    zbar::Path config_directory = home_directory / ".config" / "zbar";

    // Creates .config/zbar only if it does not exists already if not does nothing
    std::filesystem::create_directories(config_directory);
    
    zbar::Path bars_jsonc = config_directory / "bars.jsonc";
    zbar::Path settings_jsonc = config_directory / "settings.jsonc";
    zbar::Path style_toml = config_directory / "style.toml";

    // Creates missing config files (bars.jsonc, settings.jsonc, style.toml)
    if (!std::filesystem::exists(bars_jsonc)) {
        zbar::Path default_bars_jsonc = default_configs_directory / "bars.jsonc";
        
        std::filesystem::copy_file(default_bars_jsonc, bars_jsonc);
    }
    if (!std::filesystem::exists(settings_jsonc)) {
        zbar::Path default_settings_jsonc = default_configs_directory / "settings.jsonc";
        
        std::filesystem::copy_file(default_settings_jsonc, settings_jsonc);
    }
    if (!std::filesystem::exists(style_toml)) {
        zbar::Path default_style_toml = default_configs_directory / "style.toml";
        
        std::filesystem::copy_file(default_style_toml, style_toml);
    }
    

    std::cout << "home_directory: " << home_directory << std::endl;
    std::cout << "project_root: " << default_configs_directory << std::endl;

}
