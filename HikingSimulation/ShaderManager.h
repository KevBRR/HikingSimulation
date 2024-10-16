#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>
#include <iostream>

// Shader manager
// Handles the construction, compilation and destruction for the shaders, i.e Vertex Shader and Fragment Shader
// Also returns the associated shaderId when constructed

class ShaderManager {

	GLuint shaderId;

public:
	// Base ShaderManager constructor
	ShaderManager();
	// Base ShaderManager destructor - To run at runtime
	virtual ~ShaderManager();

	GLuint getShaderId() const;

protected:
	void compileShader(const char* shaderSource, GLenum shaderType);

};

#endif // !SHADER_MANAGER_H
