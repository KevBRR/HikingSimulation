#ifndef VERTEX_SHADER_MANAGER_H
#define VERTEX_SHADER_MANAGER_H

#include "ShaderManager.h"

class VertexShaderManager : public ShaderManager {

public:
	void initialize(const char* vertexSourceFile);
};


#endif // !VERTEX_SHADER_MANAGER_H

