#pragma once
#include "globalincludes.h"
//#include <ignore_me/la.h>

class WarpFunctions
{
public:
    // Uniformly sample a vector on a 2D disk with radius 1, centered around the origin
    static glm::vec3 squareToDiskUniform(const glm::vec2 &sample);

    // Concentrically sample a vector on a 2D disk with radius 1, centered around the origin
    static glm::vec3 squareToDiskConcentric(const glm::vec2 &sample);

    static float squareToDiskPDF(const glm::vec3 &sample);

    // Uniformly sample a vector on the unit sphere with respect to solid angles
    static glm::vec3 squareToSphereUniform(const glm::vec2 &sample);

    static float squareToSphereUniformPDF(const glm::vec3 &sample);

    /**
     * \brief Uniformly sample a vector on a spherical cap around (0, 0, 1)
     *
     * A spherical cap is the subset of a unit sphere whose directions
     * make an angle of less than 'theta' with the north pole. This function
     * expects the minimum angle 'theta' as a parameter.
     */
    static glm::vec3 squareToSphereCapUniform(const glm::vec2 &sample, float thetaMin);

    static float squareToSphereCapUniformPDF(const glm::vec3 &sample, float thetaMin);

    // Uniformly sample a vector on the unit hemisphere around the pole (0,0,1) with respect to solid angles
    static glm::vec3 squareToHemisphereUniform(const glm::vec2 &sample);

    static float squareToHemisphereUniformPDF(const glm::vec3 &sample);

    // Uniformly sample a vector on the unit hemisphere around the pole (0,0,1) with respect to projected solid angles
    static glm::vec3 squareToHemisphereCosine(const glm::vec2 &sample);

    static float squareToHemisphereCosinePDF(const glm::vec3 &sample);
};
