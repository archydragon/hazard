#pragma once

#ifndef HAZARD_CONFIG_H
#define HAZARD_CONFIG_H

class Config {
public:
    // default configuration values
    unsigned int windowWidth  = 1280;
    unsigned int windowHeight = 720;
    bool windowMaximized      = false;
    std::string sceneFile     = "scenes\\sample.scene";

    void load();
    void saveWindowSize(int width, int height);
    void saveWindowMaximizedState(bool state);
    void saveSceneFile(std::string pSceneFile);

private:
    bool updated = false;

    [[noreturn]] void saveFile();
};

#endif // HAZARD_CONFIG_H
