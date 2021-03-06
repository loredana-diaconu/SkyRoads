#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D {
	Mesh* CreateRectangle(std::string name, float length, float width);
	Mesh* CreateTriangle(std::string name, float length);
}

