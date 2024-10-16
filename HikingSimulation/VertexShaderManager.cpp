#include "VertexShaderManager.h"

void VertexShaderManager::initialize(const char* vertexSource) {
	return compileShader(vertexSource, GL_VERTEX_SHADER);
}