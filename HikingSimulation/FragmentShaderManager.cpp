#include "FragmentShaderManager.h"

void FragmentShaderManager::initialize(const char* fragmentSource) {
	compileShader(fragmentSource, GL_FRAGMENT_SHADER);
}
