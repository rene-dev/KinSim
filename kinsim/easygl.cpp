//
//  easygl.c
//  kinsim
//
//  Created by Rene on 14/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include "easygl.h"
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

    //glTranslatef(0, 1.9, 0);
    //glRotatef(j4, 1, 0, 0);
    //glRotatef(j5+90, 0, 1, 0);
    //wireBox(0.1, 1.15, 0.7);
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

easyobj* stl(const char *filename){
    std::ifstream infile(filename);
    std::string line;
    char a[20], b[20], c[20], d[20], e[20];
    int length = 0;
    struct vec vert;
    struct path* vertlist= 0;
    struct path* tmp;
    struct easyobj* obj = 0;
    GLfloat* result = 0;
    GLfloat* normal = 0;
    int* indices = 0;
    int index = 0;
    int vertices = 0;

    //glBegin(GL_TRIANGLES);
    
    while (getline(infile,line))
    {
        if(sscanf (line.c_str(),"%s %s %s %s %s",a,b,c,d,e)){
            //if (!strcmp("facet", a)) {
                //vert.joint_pos[0] = atof(c);
                //vert.joint_pos[1] = atof(e);
                //vert.joint_pos[2] = atof(d);
                //glNormal3f(vert.joint_pos[0],vert.joint_pos[1],vert.joint_pos[2]);
                //strcpy(a, "");
            //}
            if (!strcmp("vertex", a)) {
                vert.axis_pos[0] = atof(b)/10;
                vert.axis_pos[1] = atof(d)/10;
                vert.axis_pos[2] = atof(c)/10;
                append(&vertlist, vert);
                glVertex3f(vert.axis_pos[0],vert.axis_pos[1],vert.axis_pos[2]);
                vertices++;
                strcpy(a, "");
            }
        }
    }
    
    std::cout << "parsed " << vertices << " vertices" << std::endl;
    if(vertices == 0){
        return 0;
    }
    result = (GLfloat*)malloc(sizeof(GLfloat)*vertices*3);
    normal = (GLfloat*)malloc(sizeof(GLfloat)*vertices);
    indices = (int*)malloc(sizeof(int)*vertices);
    tmp = vertlist;
    
    while (tmp) {
        result[length]   = tmp->pos.axis_pos[0];
        result[length+1] = tmp->pos.axis_pos[1];
        result[length+2] = tmp->pos.axis_pos[2];
        length+=3;
        tmp = tmp->next;
    }
    freepath(vertlist);
    
    while (index<vertices) {
        indices[index] = index*3;
        index++;
    }
    
    obj = (easyobj*)malloc(sizeof(easyobj));
    obj->vec = result;
    obj->indices = indices;
    obj->vertices = vertices;
    //glEnd();
    return obj;
}

void draw(easyobj* obj){
    if(obj){
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3f(0.3, 0.3, 0.3);
        glVertexPointer( 3, GL_FLOAT, sizeof(*obj->vec), obj->vec);
        glDrawElements(GL_TRIANGLES, obj->vertices, GL_UNSIGNED_INT, obj->indices);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}