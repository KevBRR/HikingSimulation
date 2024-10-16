#include "VertexShaderManager.h"

void VertexShaderManager::initialize(const char* vertexSourceFile) {

	string vertexShaderBuffer;

	char vs[128] = "Shaders/vertex/";
	strcat_s(vs, vertexSourceFile);

	// Load the fragment shader into a buffer from file
	vertexShaderBuffer = loadShaderSourceFile(vs);
	if (vertexShaderBuffer.empty()) {
		cout << "Vertex Shader buffer could not be initialized" << endl;
	}

	compileShader(vertexShaderBuffer.c_str(), GL_VERTEX_SHADER);
}