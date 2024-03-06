#include "object.h"

Object::Object()
{
	std::srand(std::time(nullptr));

	std::ifstream file{"data/cube.obj"};

	if (!file)
	{
		std::cerr << "file could not be loaded\n";
		exit(1);
	}

	std::string line{};
	while (std::getline(file, line))
	{
		std::stringstream ss{line};
		std::string term{};
		ss >> term;
		if (term == "v")
		{
			glm::vec3 vertex{};
			for (int i{}; i < 3; i++)
			{
				ss >> term;
				vertex[i]=std::stof(term);
			}
			vertex_data.push_back(vertex);
		}
		
		if (term == "vn")
		{
			glm::vec3 vertex{};
			for (int i{}; i < 3; i++)
			{
				ss >> term;
				vertex[i]=std::stof(term);
			}
			normal_data.push_back(vertex);
		}

		if (term == "f")
		{
			std::vector<int> face_vertices{};
			while(ss >> term)
			{
				face_vertices.push_back(std::stoi(term.substr(0,term.find('/'))));
			}
			while (face_vertices.size() > 2)
			{
				full_vertex_data.push_back(vertex_data[face_vertices[0]-1]);
				full_vertex_data.push_back(random_color());
				// full_vertex_data.push_back(glm::vec3{1.0f});
				full_vertex_data.push_back(vertex_data[face_vertices.end()[-2]-1]);
				// full_vertex_data.push_back(glm::vec3{1.0f});
				full_vertex_data.push_back(random_color());
				full_vertex_data.push_back(vertex_data[face_vertices.end()[-1]-1]);
				// full_vertex_data.push_back(glm::vec3{1.0f});
				full_vertex_data.push_back(random_color());
				face_vertices.pop_back();
				vertex_count += 3;
			}
		}
	}


	// for (auto& v : vertex_data)
	// {
	// 	std::cout << v.x << ' ' << v.y << ' ' << v.z << '\n';
	// }
	// for (auto& v : normal_data)
	// {
	// 	std::cout << v.x << ' ' << v.y << ' ' << v.z << '\n';
	// }
	for (auto& v : full_vertex_data)
	{
		std::cout << v.x << ' ' << v.y << ' ' << v.z << '\n';
	}
}

glm::vec3 Object::random_color()
{
	return glm::vec3{static_cast<float>(std::rand())/RAND_MAX, static_cast<float>(std::rand())/RAND_MAX, static_cast<float>(std::rand())/RAND_MAX};
}