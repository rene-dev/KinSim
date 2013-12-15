//
//  easygl.h
//  kinsim
//
//  Created by Rene on 14/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#ifndef kinsim_easygl_h
#define kinsim_easygl_h

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include <math.h>
#include "path.h"

struct easyobj{
    int* indices;
    int vertices;
    GLfloat* vec;
};

typedef struct {
    GLdouble x,y,z;
} recVec;

typedef struct {
	recVec viewPos; // View position
	recVec viewDir; // View direction vector
	recVec viewUp; // View up direction
	recVec rotPoint; // Point to rotate about
	GLdouble aperture; // pContextInfo->camera aperture
	GLint viewWidth, viewHeight; // current window/screen height and width
} recCamera;

void wireBox(GLdouble width, GLdouble height, GLdouble depth);
void drawpath(struct path* currentPath);
void drawrobot(float j1,float j2,float j3,float j4,float j5,float j6);
void drawaxis();
void drawgrid();
easyobj* stl(const char *filename);
void draw(easyobj*);
void easyinit(recCamera* camera,recVec* cam,GLfloat* shapeSize,int* frame,int* curr_pos,int* display);
void resetCamera(recCamera* camera,recVec* cam,GLfloat* shapeSize);
void updateCamera(recCamera* camera,GLfloat* shapeSize);
void updateProjection(recCamera* camera,GLfloat* shapeSize);
void easydraw(recVec* cam,int* display,int* curr_pos,GLfloat* j1,GLfloat* j2,GLfloat* j3,outpath* p,path* currentPath);

#endif
