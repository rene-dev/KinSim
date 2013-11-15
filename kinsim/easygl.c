//
//  easygl.c
//  kinsim
//
//  Created by Rene on 14/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include "path.h"

// simple cube data
GLint cube_num_vertices = 8;

GLfloat cube_vertices [8][3] = {
    {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };

short cube_faces [6][4] = {
    {3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };

//ugly hacked cube
void wireBox(GLdouble width, GLdouble height, GLdouble depth){
    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width/2, height/2, depth/2);
    glColor3f (0, 0, 4.0);
    long f,i,fSize=1;
    for (f = 0; f < 6; f++) {
        glBegin (GL_LINE_LOOP);
        for (i = 0; i < 4; i++)
            glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
        glEnd ();
    }
    glPopMatrix();
}

void drawpath(struct path* currentPath){
    glBegin(GL_LINE_STRIP);
    glColor3f(1, 1, 0);
    struct path* tmp = currentPath;
    while(tmp){
        glVertex3f(tmp->pos.axis_pos[0] / 100, tmp->pos.axis_pos[2] / 100, tmp->pos.axis_pos[1] / 100);
        tmp = tmp->next;
    }
    glEnd();
}

void drawrobot(float j1,float j2,float j3,float j4,float j5,float j6){
    j1 = - j1 - 90;
    j2 = j2 - 45;
    j3 = j3 - j2 - 45;

    glPushMatrix();
    glRotatef(j1, 0, 1, 0);
    wireBox(0.5, 2.45, 0.5);

    glTranslatef(0, 2.45, 0);
    glRotatef(j2, 1, 0, 0);
    wireBox(0.5, 1.9, 0.5);

    glTranslatef(0, 1.9, 0);
    glRotatef(j3, 1, 0, 0);
    wireBox(0.5, 1.9, 0.5);

    glTranslatef(0, 1.9, 0);
    glRotatef(j4, 1, 0, 0);
    glRotatef(j5+90, 0, 1, 0);
    wireBox(0.1, 1.15, 0.7);
    glPopMatrix();
}

void drawaxis(){
    // Draw a red x-axis, a green y-axis, and a blue z-axis.
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
    glEnd();
}

void drawgrid(){
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (GLfloat i = -2; i <= 2; i += 1) {
        glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
        glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }
    glEnd();
}
