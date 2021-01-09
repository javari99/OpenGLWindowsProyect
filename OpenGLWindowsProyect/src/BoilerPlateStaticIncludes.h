#pragma once

//We will use the DLL version of glfw
#define GLFW_DLL
//Include GLAD and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//Include the DLL for the linker
#pragma comment(lib, "glfw3dll.lib")


//Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <iomanip>