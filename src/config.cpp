#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

#include <nlohmann/json.hpp>

#include "config.h"

using json = nlohmann::json;

const char* configfile = "config.json";
const int saveInterval = 5;

[[noreturn]] void ConfigWorker::saveFile() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(saveInterval));
        // If there were no updates we need to write, just keep sleeping.
        if (!updated) {
            continue;
        }

        updated = false;

        json j;
        j["windowWidth"]     = windowWidth;
        j["windowHeight"]    = windowHeight;
        j["windowMaximized"] = windowMaximized;

        std::ofstream ofs(configfile);
        ofs << std::setw(4) << j.dump(2) << std::endl;
    }
}

void ConfigWorker::load() {
    std::ifstream ifs(configfile);
    if (ifs.good()) {
        json j;
        ifs >> j;

        windowWidth     = j["windowWidth"].get<int>();
        windowHeight    = j["windowHeight"].get<int>();
        windowMaximized = j["windowMaximized"].get<bool>();
    } else {
        std::cerr << "Failed to open config file." << std::endl;
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
