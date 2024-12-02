#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>

struct heightRet {
    std::vector<float> vertices;
    unsigned int NUM_STRIPS;
    unsigned int NUM_VERTS_PER_STRIP;
    std::vector<unsigned int> indices;
    int mapWidth;  
    int mapHeight; 
};

class Heightmap {
    public:
    heightRet initialize(const char* heightMapPath);
};

#endif // !HEIGHTMAP_H
