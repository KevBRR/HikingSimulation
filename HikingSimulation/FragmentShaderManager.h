#ifndef FRAGMENT_SHADER_MANAGER_H
#define FRAGMENT_SHADER_MANAGER_H

#include "ShaderManager.h"

class FragmentShaderManager : public ShaderManager {
	
public:
	void initialize(const char* fragmentSource);
};

#endif // !FRAGMENT_SHADER_MANAGER_H
