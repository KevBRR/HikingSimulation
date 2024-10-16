#include "ShaderManager.h"

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