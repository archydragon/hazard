#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

#include <nlohmann/json.hpp>

#include "config.h"

using json = nlohmann::json;

const char* configfile = "config.json";
const int saveInterval = 5;

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ConfigWorker, windowWidth, windowHeight, windowMaximized)

[[noreturn]] void ConfigWorker::saveFile() {
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

void ConfigWorker::load() {
    std::ifstream ifs(configfile);
    if (ifs.good()) {
        std::cout << "Using " << configfile << " to read application configuration." << std::endl;
        json j;
        ifs >> j;
        from_json(j, *this);
    } else {
        std::cerr << "Failed to open config file, using default settings." << std::endl;
    }

    std::thread worker_thread(&ConfigWorker::saveFile, this);
    worker_thread.detach();
}

void ConfigWorker::saveWindowSize(int width, int height) {
    windowWidth  = width;
    windowHeight = height;
    updated      = true;
}

void ConfigWorker::saveWindowMaximizedState(bool state) {
    windowMaximized = state;
    updated         = true;
}
