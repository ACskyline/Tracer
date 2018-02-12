#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <assert.h>
#include <string.h>
#include <QString>

#define BIAS 0.0001

#define SAMPLE 100

#define BOUNCEMAX 4

#define AOMAXLENGTH 1

#define SAMPLEMODE STRATIFIED

typedef glm::vec3 Color3f;
typedef glm::vec3 Point3f;
typedef glm::vec3 Normal3f;
typedef glm::vec2 Point2f;
typedef glm::ivec2 Point2i;
typedef glm::ivec3 Point3i;
typedef glm::vec3 Vector3f;
typedef glm::vec2 Vector2f;
typedef glm::ivec2 Vector2i;
