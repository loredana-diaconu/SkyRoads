#include "Object2D.h"
#include <Core/Engine.h>
#include <iostream>

// Creeaza un triunghi
Mesh* Object2D::CreateTriangle(std::string name, float length) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, -length, 0)),
		VertexFormat(glm::vec3(length * 3 / 4, 0, 0)),
		VertexFormat(glm::vec3(0, length, 0))
	};

	std::vector<unsigned short> indices = { 0, 1, 2 };

	Mesh* triangle = new Mesh(name);
	triangle->SetDrawMode(GL_TRIANGLES);
	triangle->InitFromData(vertices, indices);
	return triangle;
}

// Creeaza un dreptunghi
Mesh* Object2D::CreateRectangle(std::string name, float length, float width) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, 0, 0)),
		VertexFormat(glm::vec3(length, width, 0)),
		VertexFormat(glm::vec3(0, width, 0))
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	Mesh* rectangle = new Mesh(name);
	rectangle->InitFromData(vertices, indices);
	return rectangle;
}
