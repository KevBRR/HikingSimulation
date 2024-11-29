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


// Function declarations
Bounds calculateBounds(const std::vector<glm::vec3>& hikePath);

std::vector<glm::vec3> hiker_conversion(const std::vector<glm::vec3>& hikePath, int heightmapWidth, int heightmapHeight, float yScale, float yShift);
std::vector<glm::vec3> validatePath(const std::vector<glm::vec3>& pathPoints, const heightRet& terrain, float horizontalScale, float yScale, float yShift);
std::vector<glm::vec3> parseGPX(const std::string& filename);

#endif // PATH_H
