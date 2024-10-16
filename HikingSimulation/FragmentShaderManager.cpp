#include "FragmentShaderManager.h"

void FragmentShaderManager::initialize(const char* fragmentSourceFile) {
	
	string fragmentShaderBuffer;
		
	char vs[128] = "Shaders/fragment/";
	strcat_s(vs, fragmentSourceFile);

	// Load the vertex shader into a buffer from file
	fragmentShaderBuffer = loadShaderSourceFile(vs);
	if (fragmentShaderBuffer.empty()) {
		cout << "Vertex Shader buffer could not be initialized" << endl;
	}

	compileShader(fragmentShaderBuffer.c_str(), GL_FRAGMENT_SHADER);
}
