#include "integrator.h"

void Integrator::run()
{
    Render();
}

void Integrator::Render()
{
    // Compute the bounds of our sample, clamping to screen's max bounds if necessary
    // Instantiate a FilmTile to store this thread's pixel colors
    std::vector<Point2i> tilePixels = bounds.GetPoints();
    // For every pixel in the FilmTile:
    for(Point2i pixel : tilePixels)
    {
        ///Uncomment this to debug a particular pixel within this tile
//        if(pixel.x != 200 && pixel.y != 200)
//        {
//            continue;
//        }
        Color3f pixelColor(0.f);
        // Ask our sampler for a collection of stratified samples, then raycast through each sample
        std::vector<Point2f> pixelSamples = sampler->GenerateStratifiedSamples();
        for(Point2f sample : pixelSamples)
        {
            sample = sample + Point2f(pixel); // _sample_ is [0, 1), but it needs to be translated to the pixel's origin.
            // Generate a ray from this pixel sample
            Ray ray = camera->Raycast(sample);
            // Get the L (energy) for the ray by calling Li(ray, scene, tileSampler, arena)
            // Li is implemented by Integrator subclasses, like DirectLightingIntegrator
            Color3f L = Li(ray, *scene, sampler, recursionLimit);
            // Accumulate color in the pixel
            pixelColor += L;
        }
        // Average all samples' energies
        pixelColor /= pixelSamples.size();

        //if(pixelColor.r > 1 || pixelColor.g > 1 || pixelColor.b > 1)
            //pixelColor = glm::vec3(1.0, 0.0, 0.0);

        film->SetPixelColor(pixel, glm::clamp(pixelColor, 0.f, 1.f));
    }
    //We're done here! All pixels have been given an averaged color.
}


void Integrator::ClampBounds()
{
    Point2i max = bounds.Max();
    max = Point2i(std::min(max.x, film->bounds.Max().x), std::min(max.y, film->bounds.Max().y));
    bounds = Bounds2i(bounds.Min(), max);
}

float BalanceHeuristic(int nf, Float fPdf, int ng, Float gPdf)
{
    //TODO
    return (nf * fPdf) / (nf*fPdf+ng*gPdf);
}

float PowerHeuristic(int nf, Float fPdf, int ng, Float gPdf)
{
    //TODO
    float f = nf * fPdf, g = ng * gPdf;
    return (f*f)/(f*f+g*g);
}
