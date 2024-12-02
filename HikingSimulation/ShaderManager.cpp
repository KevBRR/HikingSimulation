#include "ShaderManager.h"
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;

// Constructor

ShaderManager::ShaderManager() : shaderId(0) {}

// Deconstructor - virtual runtime

ShaderManager::~ShaderManager() {
	if (shaderId != 0) {
		glDeleteShader(shaderId);
	}
}

// Functions

GLuint ShaderManager::getShaderId() const {
	return shaderId;
}

// GLenum -> GL_VERTEX_SHADER / GL_FRAGMENT_SHADER
void ShaderManager::compileShader(const char* shaderSource, GLenum shaderType) {
	shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shaderSource, nullptr);
	glCompileShader(shaderId);

	// Make this bool and handle if error
}

// GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
// glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
// glCompileShader(vertexShader);

// Load shaders from file
// Ability to load shader code at runtime instead of only when compilating
string ShaderManager::loadShaderSourceFile(const string& sFileName) {
	// Load the shader program as a single string
	string output = "";
	string line = "";

	ifstream shaderFile(sFileName);

	if (!shaderFile.is_open()) {
		throw runtime_error("Could not load shader file: " + sFileName);
	}

	stringstream buffer;
	buffer << shaderFile.rdbuf();

	return buffer.str();
}


