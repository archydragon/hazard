#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include <nlohmann/json.hpp>
#include <utility>

#include "Config.h"

using json = nlohmann::json;

const char* configfile = "config.json";
const int saveInterval = 5;

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, windowWidth, windowHeight, windowMaximized, sceneFile)

[[noreturn]] void Config::saveFile() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(saveInterval));
        // If there were no updates we need to write, just keep sleeping.
        if (!updated) {
            continue;
        }

        updated = false;

        json j;
        to_json(j, *this);

        std::ofstream ofs(configfile);
        ofs << std::setw(4) << j.dump(2) << std::endl;
        std::cout << "Configuration saved." << std::endl;
    }
}

void Config::load() {
    std::ifstream ifs(configfile);
    if (ifs.good()) {
        std::cout << "Using " << configfile << " to read application configuration." << std::endl;
        json j;
        ifs >> j;
        from_json(j, *this);
    } else {
        std::cerr << "Failed to open config file, using default settings." << std::endl;
        // Trigger config update to save even default settings.
        updated = true;
    }

    std::thread worker_thread(&Config::saveFile, this);
    worker_thread.detach();
}

void Config::saveWindowSize(int width, int height) {
    windowWidth  = width;
    windowHeight = height;
    updated      = true;
}

void Config::saveWindowMaximizedState(bool state) {
    windowMaximized = state;
    updated         = true;
}

void Config::saveSceneFile(std::string pSceneFile) {
    sceneFile = std::move(pSceneFile);
    updated   = true;
}
