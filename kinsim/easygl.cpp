//
//  easygl.c
//  kinsim
//
//  Created by Rene, ands on 14/11/13.
//  Copyright (c) 2013 Rene, ands. All rights reserved.
//

#include "easygl.h"

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

	glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LINE_SMOOTH);
    //glLineWidth(2.0f);

    //sphere.load("sphere.stl", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
}

void easygl::draw(float period)
{
    // update camera
    glm::vec3 direction = glm::rotate(orientation, glm::vec3(0, 0, -1));
    glm::vec3 right = glm::cross(up, direction);
    glm::vec3 up = glm::cross(direction, right);
    position -= (movement.z * direction + movement.x * right + movement.y * up) * speed * (float)period;
    target = position + direction;
    
    
	glViewport(0, 0, viewportSize.x, viewportSize.y);
	if(viewportSize.y == 0)
		viewportSize.y = 1;
	aspectRatio = (float)viewportSize.x / (float)viewportSize.y;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(glm::value_ptr(glm::perspective(fieldOfView, aspectRatio, near, far)));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(glm::lookAt(position, target, up)));

    // light
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::vec4(0.6f, 0.7f, 0.7f, 0.0f)));
	glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(glm::vec4(1.0f, 0.9f, 0.8f, 1.0f)));
	glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// scene
    drawGrid();
    drawRobot();
    drawAxis();
    drawPath();

    glFlush();
}

void easygl::scroll(double offset){
    double deltaAperture = offset * -fieldOfView / 200.0;
    fieldOfView += deltaAperture;
    if (fieldOfView < 0.1) // do not let aperture <= 0.1
        fieldOfView = 0.1;
    if (fieldOfView > 179.9) // do not let aperture >= 180
        fieldOfView = 179.9;
}

void easygl::drawBox(GLdouble width, GLdouble height, GLdouble depth)
{
	const GLfloat vertices[8][3] = {
		{1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
		{1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };
	const GLfloat normals[6][3] = {
		{0.0, 0.0, 1.0}, {-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
		{0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}};
	const short faces[6][4] = {
		{3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };

	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glm::value_ptr(glm::vec4(0.5f, 0.5f, 1.0f, 1.0f)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 2.0f);

    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width/2, height/2, depth/2);
    glBegin(GL_QUADS);
    for(int f = 0; f < 6; f++)
	{
		glNormal3fv(normals[f]);
        for(int i = 0; i < 4; i++)
            glVertex3fv(vertices[faces[f][i]]);
	}
    glEnd ();
    glPopMatrix();

    glDisable(GL_LIGHTING);
}

void easygl::drawPath()
{
	if(!currentPath)
		return;

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
	if(!robotState)
		return;

    float j1 = -robotState->joint_pos[0] - 90;
    float j2 = robotState->joint_pos[1] - 45;
    float j3 = robotState->joint_pos[2] - j2 - 45;

    glPushMatrix();
    glRotatef(j1, 0, 1, 0);
    drawBox(0.5, 2.45, 0.5);
    sphere.draw();

    glTranslatef(0, 2.45, 0);
    glRotatef(j2, 1, 0, 0);
    drawBox(0.5, 1.9, 0.5);
    sphere.draw();

    glTranslatef(0, 1.9, 0);
    glRotatef(j3, 1, 0, 0);
    drawBox(0.5, 1.9, 0.5);
    sphere.draw();

    //glTranslatef(0, 1.9, 0);
    //glRotatef(j4, 1, 0, 0);
    //glRotatef(j5+90, 0, 1, 0);
    //drawBox(0.1, 1.15, 0.7);
    //sphere.draw();
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
