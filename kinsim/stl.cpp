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
    std::ifstream infile(filename.c_str());

    while (!infile.eof())
    {
    	std::string tmp;
    	infile >> tmp;

    	glm::vec3 normal;

    	if(tmp == "normal")
    	{
    		infile >> normal.x >> normal.y >> normal.z;
    	}
    	else if(tmp == "vertex")
    	{
    		glm::vec3 vertex;
    		infile >> vertex.x >> vertex.y >> vertex.z;
    		vertices.push_back(vertex * 0.1f);
    		normals.push_back(normal);
    	}
    }

    infile.close();

    std::cout << "parsed " << normals.size() << " normals, " << vertices.size() << " vertices." << std::endl;

    this->color = color;
}

