#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include "VertexShaderManager.h"
#include "FragmentShaderManager.h"
#include "camera.h"
#include "Heightmap.h"
#include <vector>
#include "Path.h"
#include <limits>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Bounds calculateBounds(const std::vector<glm::vec3>& hikePath) {
    Bounds bounds;

    // Getting the minimum and the maximum from the path to set the boundaries
    bounds.minLat = std::numeric_limits<float>::max();
    bounds.maxLat = std::numeric_limits<float>::lowest();
    bounds.minLon = std::numeric_limits<float>::max();
    bounds.maxLon = std::numeric_limits<float>::lowest();
    bounds.minHeight = std::numeric_limits<float>::max();
    bounds.maxHeight = std::numeric_limits<float>::lowest();

    for (const auto& point : hikePath) {
        float latitude = point.x;
        float longitude = point.y;
        float elevation = point.z;

        if (latitude < bounds.minLat) bounds.minLat = latitude;
        if (latitude > bounds.maxLat) bounds.maxLat = latitude;
        if (longitude < bounds.minLon) bounds.minLon = longitude;
        if (longitude > bounds.maxLon) bounds.maxLon = longitude;
        if (elevation < bounds.minHeight) bounds.minHeight = elevation;
        if (elevation > bounds.maxHeight) bounds.maxHeight = elevation;
    }

    return bounds;
}


std::vector<glm::vec3> processHikePath(const std::vector<glm::vec3>& hikePath, int heightmapWidth, int heightmapHeight, float elevationScale, float elevationShift, const heightRet& terrain, float horizontalScale) {
    std::vector<glm::vec3> processedPath;

    if (hikePath.empty()) return processedPath;

    processedPath.reserve(hikePath.size());

    // dimensions
    float terrainWidth = terrain.mapWidth * horizontalScale;
    float terrainDepth = terrain.mapHeight * horizontalScale;

    float minX = -terrainWidth * 0.5f;
    float maxX = terrainWidth * 0.5f;
    float minZ = -terrainDepth * 0.5f;
    float maxZ = terrainDepth * 0.5f;

    // Calculate bounds of hike path, the limits basically
    Bounds bounds = calculateBounds(hikePath);

    // Getting the min/max lat and lon from the hiking path
    float minLat = bounds.minLat;
    float maxLat = bounds.maxLat;
    float minLon = bounds.minLon;
    float maxLon = bounds.maxLon;


    for (const auto& point : hikePath) {
        float latitude = point.x;   // x is latitude
        float longitude = point.y;  // y is longitude
        float elevation = point.z;  // z is elevation - y and z gets switched as the z from the hike vector is the elevation

        // Map latitude/longitude z-axisand x-axis
        float z = (latitude - minLat) / (maxLat - minLat) * terrainDepth - terrainDepth / 2.0f;
        float x = (longitude - minLon) / (maxLon - minLon) * terrainWidth - terrainWidth / 2.0f;

        // clamp x and z to the terrain boundaries
        x = glm::clamp(x, minX, maxX);
        z = glm::clamp(z, minZ, maxZ);

        // map elevation based on the heightmap and apply scaling if i need it 
        float y = elevation * elevationScale - elevationShift;

        // using the generated terrain height data to adjust elevation
        int xIndex = static_cast<int>((x - minX) / horizontalScale);
        int zIndex = static_cast<int>((z - minZ) / horizontalScale);

        // here to prevent a crash in case any of the vectors are out of the range
        if (xIndex < 0 || xIndex >= terrain.mapWidth) {
            continue;
        }
        if (zIndex < 0 || zIndex >= terrain.mapHeight) {
            continue;
        }

        int vertexIndex = (zIndex * terrain.mapWidth + xIndex) * 3 + 1; // Index for heightmap y coordinate
        float terrainHeightAtPoint = terrain.vertices[vertexIndex];

        // clamp y to stay between the terrain height and a buffer above it
        float minHeight = terrainHeightAtPoint;
        float maxHeight = terrainHeightAtPoint + 50.0f; // adjust as needed 
        y = glm::clamp(y, minHeight, maxHeight);
        
        processedPath.emplace_back(glm::vec3(x, y, z));
    }

    return processedPath;
}
