#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <glm/glm.hpp>
#include <random>
#include <ctime>

class Object
{
	glm::vec3 random_color();

public:
	Object();

	std::vector<glm::vec3> vertex_data{};
	std::vector<glm::vec3> normal_data{};
	std::vector<glm::vec3> full_vertex_data{};
	int vertex_count{};

};

#endif