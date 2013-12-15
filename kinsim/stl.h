//
//  stl.h
//  kinsim
//
//  Created by Rene, ands on 14/11/13.
//  Copyright (c) 2013 Rene, ands. All rights reserved.
//

#ifndef kinsim_stl_h
#define kinsim_stl_h

#include <string>
#include <vector>
#include <glm/glm.hpp>

class stl
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::vec4 color;
	
	stl();
	~stl();
	
	void load(std::string filename, glm::vec4 color);
};

#endif
