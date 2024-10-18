#include <GL/glew.h>
#include <iostream>

#include "Heightmap.h"
#include "stb_image.h"
#include <vector>

using namespace std;

int width;
int height;
int nChannels;
unsigned char* data;

// Constructor

struct heightRet
{
    std::vector<float> vertices;
    const unsigned int NUM_STRIPS;
    const unsigned int NUM_VERTS_PER_STRIP;
};

heightRet Heightmap::initialize(const char* heightMapPath) {

    

    string pathMerge;

    char vs[128] = "resources/heightmaps/";
    strcat_s(vs, heightMapPath);

    data = stbi_load(pathMerge.c_str(),
        &width, &height, &nChannels,
        0);

    // vertex generation
    vector<float> vertices;
    float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + width * i) * nChannels;
            // raw height at coordinate
            unsigned char y = texel[0];

            // vertex
            vertices.push_back(-height / 2.0f + i);        // v.x
            vertices.push_back((int)y * yScale - yShift); // v.y
            vertices.push_back(-width / 2.0f + j);        // v.z
        }
    }

    stbi_image_free(data);

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

    // Needed for rendering
    // Number of strips to be rendered 
    const unsigned int NUM_STRIPS = height - 1;
    // Number of vertices per strip 
    const unsigned int NUM_VERTS_PER_STRIP = width * 2;
}



