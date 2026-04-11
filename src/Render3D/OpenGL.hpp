#pragma once

// 1. Inclure GLAD en PREMIER (C'est lui qui définit GLuint, GL_ARRAY_BUFFER, etc.)
#include <glad/glad.h>

// 2. Ensuite, inclure quick_imgui
#include <quick_imgui/quick_imgui.hpp> 

// 3. Enfin, inclure GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <memory>
#include <iostream>