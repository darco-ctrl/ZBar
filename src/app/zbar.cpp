#include "app/zbar.hpp"

void ZBar::initialize() {
    create_bootstrap();
    create_config_manager();
    // config_manager how to give it path ? and create config_manager  or is it created already from hpp?
}

void ZBar::create_bootstrap() {
    bootstrap.initialize();
    paths = bootstrap.create_zbar_paths();
}

void ZBar::create_config_manager() {
    config_manager.initialize();
}
