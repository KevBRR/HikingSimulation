#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>

struct heightRet {
    std::vector<float> vertices;
    unsigned int NUM_STRIPS;
    unsigned int NUM_VERTS_PER_STRIP;
    std::vector<unsigned int> indices;
};

class Heightmap {
    public:
    heightRet initialize(const char* heightMapPath);

    // Need a destructor here, release the image file 
};

#endif // !HEIGHTMAP_H
