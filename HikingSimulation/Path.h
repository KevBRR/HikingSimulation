#ifndef PATH_H
#define PATH_H

#include <vector>
#include <glm/vec3.hpp>
#include <string>

// Struct to hold bounds information
struct Bounds {
    float minLat, maxLat;
    float minLon, maxLon;
    float minHeight, maxHeight;
};

// Functions
Bounds calculateBounds(const std::vector<glm::vec3>& hikePath);
std::vector<glm::vec3> processHikePath(const std::vector<glm::vec3>& hikePath, int heightmapWidth, int heightmapHeight, float yScale, float yShift, const heightRet& terrain, float horizontalScale);

#endif // PATH_H
