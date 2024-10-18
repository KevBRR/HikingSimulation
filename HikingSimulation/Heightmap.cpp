#include <GL/glew.h>
#include <iostream>
#include "Heightmap.h"
#include "stb_image.h"
#include <vector>

using namespace std;

int width;
int height;
int nChannels;
unsigned char* heightMapData;

// Constructor



heightRet Heightmap::initialize(const char* heightMapPath) {

    string pathMerge = string("resources/heightmaps/") + heightMapPath;

    //stbi_set_flip_vertically_on_load(true);
    heightMapData = stbi_load(pathMerge.c_str(),
        &width, &height, &nChannels,
        0);

    if (!heightMapData) {
        cout << "Failed to load heightmap: " << heightMapPath << endl;
        return heightRet{};
    }

    // vertex generation
    vector<float> vertices;

    // 192.0f
    float yScale = 256.0f / 256.0f, yShift = -25.0f;  // 16.0f apply a scale+shift to the height data
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = heightMapData + (j + width * i) * nChannels;
            // raw height at coordinate
            unsigned char y = texel[0];

            // vertex
            vertices.push_back(-height / 2.0f + i);        // v.x
            vertices.push_back((int)y * yScale - yShift); // v.y
            vertices.push_back(-width / 2.0f + j);        // v.z
        }
    }

    stbi_image_free(heightMapData);

    // index generation
    // Generate the indices for each triangle strip
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < height - 1; i++)       // for each row a.k.a. each strip
    {
        for (unsigned int j = 0; j < width; j++)      // for each column
        {
            for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(j + width * (i + k));
            }
        }
    }

    std::cout << "Loaded " << indices.size() << " indices" << std::endl;

    // Needed for rendering
    // Number of strips to be rendered 
    const unsigned int NUM_STRIPS = height - 1;
    // Number of vertices per strip 
    const unsigned int NUM_VERTS_PER_STRIP = width * 2;

    heightRet output;

    output.vertices = vertices;
    output.NUM_STRIPS = NUM_STRIPS;
    output.NUM_VERTS_PER_STRIP = NUM_VERTS_PER_STRIP;
    output.indices = indices;

    return output;

}



