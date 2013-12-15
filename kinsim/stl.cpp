//
//  stl.cpp
//  kinsim
//
//  Created by Rene, ands on 14/11/13.
//  Copyright (c) 2013 Rene, ands. All rights reserved.
//

#include "stl.h"
#include <iostream>
#include <fstream>

stl::stl()
{
}

stl::~stl()
{
	vertices.clear();
	normals.clear();
}

void stl::load(std::string filename, glm::vec4 color)
{
	// read vertices
    std::ifstream infile(filename.c_str());
    while (!infile.eof())
    {
    	std::string tmp;
    	infile >> tmp;
    	if(tmp == "vertex")
    	{
    		glm::vec3 vertex;
    		infile >> vertex.x >> vertex.y >> vertex.z;
    		vertices.push_back(vertex * 0.1f);
    	}
    }
    infile.close();

	// calculate normals
	normals.resize(vertices.size());
	glm::vec3 normal;
    for(int i = 0; i < vertices.size(); i++)
	{
		if(i % 3 == 0)
			normal = glm::normalize(glm::cross(vertices[i + 1] - vertices[i], vertices[i + 2] - vertices[i]));
		for(int j = 0; j < vertices.size(); j++)
		{
			if(glm::dot(vertices[i] - vertices[j], vertices[i] - vertices[j]) < 0.0001f)
			{
				normals[j] = normal;
			}
		}
	}

	// normalize normals
    for(int i = 0; i < normals.size(); i++)
		normals[i] = glm::normalize(normals[i]);

    std::cout << "parsed " << normals.size() << " normals, " << vertices.size() << " vertices." << std::endl;

    this->color = color;
}

