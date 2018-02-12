#pragma once
//#include <ignore_me/enums.h>
#include "enums.h"
#include "globalincludes.h"
#include <vector>
//#include <ignore_me/la.h>
#include "pcg32.h"



class Sampler
{
public:
    Sampler();

    void generateSamples(int& numSamples, SampleMode sampleMode, WarpMethod warpMethod);
    static glm::vec3 warpSample(glm::vec2 sample, WarpMethod warpMethod, float thetaMax);

    unsigned int getNumSamples();

    const std::vector<float>& peekSamples();
    const std::vector<float>& peekSampleColors();

    float thetaMax; // For sphere cap

private:
    std::vector<float> samples; // Every 3 floats is a point
    std::vector<float> sampleColors; // Every 3 floats is a point's color
};

// PDF of disc is 1/PI because area of disc is PI*r^2, and r = 1.
