//
//  easygl.c
//  kinsim
//
//  Created by Rene on 14/11/13.
//  Copyright (c) 2013 Rene, ands. All rights reserved.
//

#include "easygl.h"
#include "stl.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <string.h>

// simple cube data
GLint cube_num_vertices = 8;

GLfloat cube_vertices [8][3] = {
    {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };

short cube_faces [6][4] = {
    {3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };



easygl::easygl()
{
}

easygl::~easygl()
{
}

void easygl::init()
{
    position = glm::vec3(0.0f, 1.0f, 5.0f);
    target = glm::vec3(0, 1, 0);
    up = glm::vec3(0, 1, 0);
    orientation = glm::quat();

    viewportSize = glm::ivec2();
	fieldOfView = 60.0f;
	near = 0.1f, far = 1000.0f;
	aspectRatio = 1.0;

    glEnable(GL_LINE_SMOOTH);

    //obj = stl("/Users/crinq/Dropbox/maschinen/knuth/pumpe.stl");
}

void easygl::draw()
{
	glViewport(0, 0, viewportSize.x, viewportSize.y);
	if(viewportSize.y == 0)
		viewportSize.y = 1;
	aspectRatio = (float)viewportSize.x / (float)viewportSize.y;

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(glm::value_ptr(glm::perspective(fieldOfView, aspectRatio, near, far)));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(glm::lookAt(position, target, up)));

    drawGrid();
    drawRobot();
    drawAxis();
    drawPath();
    //drawStl(obj);

    glFlush();
}

void easygl::drawBox(GLdouble width, GLdouble height, GLdouble depth)
{
    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width/2, height/2, depth/2);
    glColor3f (0, 0, 1.0f);
    long f,i,fSize=1;
    for (f = 0; f < 6; f++) {
        glBegin (GL_LINE_LOOP);
        for (i = 0; i < 4; i++)
            glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
        glEnd ();
    }
    glPopMatrix();
}

void easygl::drawPath()
{
    glBegin(GL_LINE_STRIP);
    glColor3f(1, 1, 0);
    path* tmp = currentPath;
    while(tmp)
    {
        glVertex3f(tmp->pos.axis_pos[0] / 100, tmp->pos.axis_pos[2] / 100, tmp->pos.axis_pos[1] / 100);
        tmp = tmp->next;
    }
    glEnd();
}

void easygl::drawRobot()
{
    float j1 = -robotState->joint_pos[0] - 90;
    float j2 = robotState->joint_pos[1] - 45;
    float j3 = robotState->joint_pos[2] - j2 - 45;

    glPushMatrix();
    glRotatef(j1, 0, 1, 0);
    drawBox(0.5, 2.45, 0.5);

    glTranslatef(0, 2.45, 0);
    glRotatef(j2, 1, 0, 0);
    drawBox(0.5, 1.9, 0.5);

    glTranslatef(0, 1.9, 0);
    glRotatef(j3, 1, 0, 0);
    drawBox(0.5, 1.9, 0.5);

    //glTranslatef(0, 1.9, 0);
    //glRotatef(j4, 1, 0, 0);
    //glRotatef(j5+90, 0, 1, 0);
    //drawBox(0.1, 1.15, 0.7);
    glPopMatrix();
}

void easygl::drawAxis()
{
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
    glEnd();
}

void easygl::drawGrid()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (GLfloat i = -2; i <= 2; i += 1)
    {
        glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
        glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }
    glEnd();
}

void easygl::drawStl(stl* obj)
{
    if(!obj)
    	return;

	glColor4fv(glm::value_ptr(obj->color));
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3), obj->vertices.data());
    glDrawArrays(GL_TRIANGLES, 0, obj->vertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);
}
