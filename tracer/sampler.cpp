#include "sampler.h"
#include "warpfunctions.h"

Sampler::Sampler() : thetaMax(150)
{}

// This function has several steps:
// 1. Generate a sample within a [0, 1) square using the sample mode specified
//      Note that if the sample mode is grid-based or stratified, the number of samples
//      generated will be rounded to the nearest square number (and will modify the input numSamples)
// 2. Warp this sample to a new surface with the method specified, then add the sample to a collection
// 3. Repeat until you've generated a number of samples equal to numSamples
// Based on code written by Wenzel Jakob for his Nori renderer

void Sampler::generateSamples(int& numSamples, SampleMode sampleMode, WarpMethod warpMethod)
{
    // The square root of the number of samples input
    int sqrtVal = (int) (std::sqrt((float) numSamples) + 0.5);
    // A number useful for scaling a square of size sqrtVal x sqrtVal to 1 x 1
    float invSqrtVal = 1.f / sqrtVal;

    // Ensure that the number of samples we use fits evenly within a square grid
    if(sampleMode == GRID || sampleMode == STRATIFIED)
    {
        numSamples = sqrtVal * sqrtVal;
    }
    samples.resize(numSamples * 3); // 3 floats for position
    sampleColors.resize(numSamples * 3); // 3 floats for color
    float colorScale = 1.f / numSamples; // For creating a gradient

    pcg32 rng; // We'll be using the PCG random number generator class to generate samples.
    // You can read more about this random number generator at http://www.pcg-random.org/
    // PBRT ed. 3 also discusses the PCG RNG from pages 1065 - 1066

    for(int i = 0; i < numSamples; ++i)
    {
        int y = i / sqrtVal;
        int x = i % sqrtVal;
        glm::vec2 sample;

        switch(sampleMode)
        {
        case RANDOM:
            sample = glm::vec2(rng.nextFloat(), rng.nextFloat());
            break;

        case GRID:
            //TODO
            sample = invSqrtVal*glm::vec2(x + 0.5f, y + 0.5f);
            //throw std::runtime_error("You haven't yet implemented grid sampling!");
            break;

        case STRATIFIED:
            //TODO
            sample = invSqrtVal*glm::vec2(x+rng.nextFloat(),y+rng.nextFloat());
            //throw std::runtime_error("You haven't yet implemented stratified sampling!");
            break;
        }


        glm::vec3 warpResult = warpSample(sample, warpMethod, thetaMax);

        samples[i * 3 + 0] = warpResult.x;
        samples[i * 3 + 1] = warpResult.y;
        samples[i * 3 + 2] = warpResult.z;

        // Color needs to gradiate over the course of 1 column of samples

        sampleColors[i * 3 + 1] = i * colorScale;
        sampleColors[i * 3 + 2] = 1 - i * colorScale;
        sampleColors[i * 3 + 0] = 1;
    }
}

// Takes the samples obtained from generateSamples and warps them to the shape provided
glm::vec3 Sampler::warpSample(glm::vec2 sample, WarpMethod warpMethod, float thetaMax)
{
    switch(warpMethod)
    {
    case NONE:
        return glm::vec3(sample, 0);
        break;

    case DISC_UNIFORM:
        return WarpFunctions::squareToDiskUniform(sample);
        break;

    case DISK_CONCENTRIC:
        return WarpFunctions::squareToDiskConcentric(sample);
        break;

    case SPHERE:
        return WarpFunctions::squareToSphereUniform(sample);
        break;

    case SPHERE_CAP:
        return WarpFunctions::squareToSphereCapUniform(sample, thetaMax);
        break;

    case HEMISPHERE_UNIFORM:
        return WarpFunctions::squareToHemisphereUniform(sample);
        break;

    case HEMISPHERE_COSINE:
        return WarpFunctions::squareToHemisphereCosine(sample);
        break;
    }

    return glm::vec3(sample, 0);
}

unsigned int Sampler::getNumSamples()
{
    return this->samples.size() / 3;
}

const std::vector<float>& Sampler::peekSamples()
{
    return this->samples;
}

const std::vector<float>& Sampler::peekSampleColors()
{
    return this->sampleColors;
}
