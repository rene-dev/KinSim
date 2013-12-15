//
//  stl.cpp
//  kinsim
//
//  Created by Rene, ands on 14/11/13.
//  Copyright (c) 2013 Rene, ands. All rights reserved.
//

#include "stl.h"

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

void stl::draw()
{
    if(!vertices.size())
    	return;
    
	glEnable(GL_LIGHTING);
    
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glm::value_ptr(color));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 16.0f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3), vertices.data());
	glNormalPointer(GL_FLOAT, sizeof(glm::vec3), normals.data());
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glDisable(GL_LIGHTING);
}
