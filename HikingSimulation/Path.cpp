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

std::vector<glm::vec3> hiker_conversion(const std::vector<glm::vec3>& hikePath, int heightmapWidth, int heightmapHeight, float yScale, float yShift) {
    std::vector<glm::vec3> convertedHikePath;
    float terrainWidth = static_cast<float>(heightmapWidth);
    float terrainHeight = static_cast<float>(heightmapHeight);

    Bounds bounds = calculateBounds(hikePath);

    float minLat = bounds.minLat;
    float maxLat = bounds.maxLat;
    float minLon = bounds.minLon;
    float maxLon = bounds.maxLon;
    float minHeight = bounds.minHeight;
    float maxHeight = bounds.maxHeight;

    //convertedHikePath.clear();

    for (const auto& point : hikePath) {
        float latitude = point.x;
        float longitude = point.y;
        float elevation = point.z;

        // GPS coordinates / normalized terrain/height
        float z = (latitude - minLat) / (maxLat - minLat) * terrainHeight - terrainHeight / 2.0f; // set lat to z
        float x = (longitude - minLon) / (maxLon - minLon) * terrainWidth - terrainWidth / 2.0f;  // set lon to x

        float y = elevation * yScale - yShift;

        // fit terrain bounds
        x = glm::clamp(x, -terrainWidth / 2.0f, terrainWidth / 2.0f);
        z = glm::clamp(z, -terrainHeight / 2.0f, terrainHeight / 2.0f);
        y = glm::clamp(y, minHeight, maxHeight);

        convertedHikePath.emplace_back(glm::vec3(x, y, z));
    }

    return convertedHikePath;

    //cout << "converted " << convertedHikePath.size() << " path points " << std::endl;
}
std::vector<glm::vec3> validatePath(const std::vector<glm::vec3>& pathPoints, const heightRet& terrain, float horizontalScale, float yScale, float yShift) {
    std::vector<glm::vec3> validatedPath;
    if (pathPoints.empty()) return validatedPath;

    validatedPath.reserve(pathPoints.size());

    float terrainWidth = terrain.mapWidth * horizontalScale;
    float terrainDepth = terrain.mapHeight * horizontalScale;
    float minX = -terrainWidth * 0.5f;
    float maxX = terrainWidth * 0.5f;
    float minZ = -terrainDepth * 0.5f;
    float maxZ = terrainDepth * 0.5f;

    for (auto& point : pathPoints) {
        // clamping the coordinates to the terrain
        float x = glm::clamp(point.x, minX, maxX);
        float z = glm::clamp(point.z, minZ, maxZ);

        // setting indices based on terrain grid itself
        int xIndex = static_cast<int>((x - minX) / horizontalScale);
        int zIndex = static_cast<int>((z - minZ) / horizontalScale);

        if (xIndex < 0 || xIndex >= terrain.mapWidth || zIndex < 0 || zIndex >= terrain.mapHeight) {
            std::cerr << "Path point out of bounds: (" << x << ", " << z << ")" << std::endl;
            continue;
        }

        // At each given point, we compute the terrain height to give us y
        int vertexIndex = (zIndex * terrain.mapWidth + xIndex) * 3 + 1; // index y coordinate
        float terrainHeight = terrain.vertices[vertexIndex];

        // Adjust the height to ensure the path is above the terrain
        float y = terrainHeight + 1.0f; // lift off the ground
        validatedPath.emplace_back(glm::vec3(x, y, z));
    }

    return validatedPath;
}

std::vector<glm::vec3> processHikePath(const std::vector<glm::vec3>& hikePath,
    int heightmapWidth,
    int heightmapHeight,
    float elevationScale,
    float elevationShift,
    const heightRet& terrain,
    float horizontalScale) {
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
        float elevation = point.z;  // z is elevation

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

        if (xIndex < 0 || xIndex >= terrain.mapWidth || zIndex < 0 || zIndex >= terrain.mapHeight) {
            std::cerr << "Path point out of bounds: (" << x << ", " << z << ")" << std::endl;
            continue;
        }

        int vertexIndex = (zIndex * terrain.mapWidth + xIndex) * 3 + 1; // Index for heightmap's Y coordinate
        float terrainHeightAtPoint = terrain.vertices[vertexIndex];

        // clamp y to stay between the terrain height and a buffer above it
        float minHeight = terrainHeightAtPoint;
        float maxHeight = terrainHeightAtPoint + 50.0f; // adjust as needed 
        y = glm::clamp(y, minHeight, maxHeight);

        processedPath.emplace_back(glm::vec3(x, y, z));
    }

    return processedPath;
}




