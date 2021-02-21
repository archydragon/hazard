#ifndef HAZARD_RENDERSTATS_H
#define HAZARD_RENDERSTATS_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    std::vector<double> Data;
    double Max  = 0;
    int lastMax = 0;

    ScrollingBuffer() {
        MaxSize = 300;
        Offset  = 0;
        Data.reserve(MaxSize);
        AddPoint(0);
    }

    void AddPoint(double v) {
        // This magic is needed for dynamic adjustment of graph size.
        lastMax++;
        if (v > Max && lastMax < MaxSize) {
            Max = v;
        }
        if (lastMax > MaxSize) {
            Max     = Max * 0.7;
            lastMax = 0;
        }

        if (Data.size() < MaxSize)
            Data.push_back(v);
        else {
            Data[Offset] = v;
            Offset       = (Offset + 1) % MaxSize;
        }
    }
};

class RenderStats {
public:
    unsigned int fps       = 0;
    double frameTime       = 0;
    unsigned int drawCalls = 0;

    ScrollingBuffer frameTimeHistory;

    void refresh() {
        drawCalls = 0;
        nFrames++;
        double currentTime = glfwGetTime();

        frameTime     = (currentTime - lastFrameTime) * 1000;
        lastFrameTime = currentTime;
        frameTimeHistory.AddPoint(frameTime);

        float deltaSecondTime = float(currentTime - lastSecondTime);
        if (deltaSecondTime >= 1.0) {
            fps     = nFrames;
            nFrames = 0;
            lastSecondTime += 1.0;
        }
    }

private:
    double lastFrameTime  = glfwGetTime();
    double lastSecondTime = glfwGetTime();
    int nFrames           = 0;
};

#endif // HAZARD_RENDERSTATS_H
