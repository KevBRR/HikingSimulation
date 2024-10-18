#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

class Heightmap {

    int width;
    int height;
    int nChannels;
    unsigned char* data;

public:
    initialize(const char* heightMapPath);

    // Need a destructor here, release the image file 



};

#endif // !HEIGHTMAP_H
