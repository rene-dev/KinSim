//
//  easygl.c
//  kinsim
//
//  Created by Rene on 14/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#include "easygl.h"
#include <stdlib.h>
#include <string.h>

// simple cube data
GLint cube_num_vertices = 8;

GLfloat cube_vertices [8][3] = {
    {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };

short cube_faces [6][4] = {
    {3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };

void easydraw(recVec* cam,int* display,int* curr_pos,GLfloat* j1,GLfloat* j2,GLfloat* j3,outpath* p,path* currentPath){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, -1, -5);
    glRotatef(cam->y, 1, 0, 0);
    glRotatef(cam->x, 0, 1, 0);
    glRotatef(0, 0, 0, 1);
    
    // Set every pixel in the frame buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    if(display){
        //j1 = [joint1 floatValue]+p.jointpos1[curr_pos];
        //j2 = [joint2 floatValue]+p.jointpos2[curr_pos];
        //j3 = [joint3 floatValue]+p.jointpos3[curr_pos]-360;
        *j1 = p->jointpos1[*curr_pos];
        *j2 = p->jointpos2[*curr_pos];
        *j3 = p->jointpos3[*curr_pos]-360;
        //[debugtext setStringValue:[NSString stringWithFormat:@"%f\n%f\n%f",*j1,*j2,*j3]];
        
        drawgrid();
        drawrobot(*j1, *j2, *j3, 0, 0, 0);
        drawaxis();
        drawpath(currentPath);
        //draw(obj);
        
        // Flush drawing command buffer to make drawing happen as soon as possible.
        glFlush();
        /*
        *curr_pos += (int)[speed floatValue];
        if(*curr_pos < 0){
            *curr_pos = p.length - 1;
        }
        if(*curr_pos >= p.length){
            *curr_pos = 0;
        }
        if([speed floatValue] != 0){
            [pos setFloatValue:curr_pos * 100 / p.length];
        }else{
            curr_pos = [pos floatValue]/100*(p.length-1);
        }
        */
    }
}

void updateCamera(recCamera* camera,GLfloat* shapeSize){
    glViewport (0, 0, camera->viewWidth, camera->viewHeight);
    updateProjection(camera,shapeSize);  // update projection matrix
}

// update the projection matrix based on camera and view info
void updateProjection(recCamera* camera,GLfloat* shapeSize){
	GLdouble ratio, radians, wd2;
	GLdouble left, right, top, bottom, near, far;
    
    //[[self openGLContext] makeCurrentContext];
    
	// set projection
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	near = -camera->viewPos.z - *shapeSize * 0.5;
	if (near < 0.00001)
		near = 0.00001;
        far = -camera->viewPos.z + *shapeSize * 0.5;
        if (far < 1.0)
            far = 1.0;
            radians = 0.0174532925 * camera->aperture / 2; // half aperture degrees to radians
            wd2 = near * tan(radians);
            ratio = camera->viewWidth / (float) camera->viewHeight;
            if (ratio >= 1.0) {
                left  = -ratio * wd2;
                right = ratio * wd2;
                top = wd2;
                bottom = -wd2;
            } else {
                left  = -wd2;
                right = wd2;
                top = wd2 / ratio;
                bottom = -wd2 / ratio;
            }
	glFrustum (left, right, bottom, top, near, far);
}

// sets the camera data to initial conditions
void resetCamera(recCamera* camera,recVec* cam,GLfloat* shapeSize){
    camera->aperture = 40;
    //camera.rotPoint = gOrigin;
    
    camera->viewPos.x = 0.0;
    camera->viewPos.y = 0.0;
    camera->viewPos.z = 0.0;
    camera->viewDir.x = -camera->viewPos.x;
    camera->viewDir.y = -camera->viewPos.y;
    camera->viewDir.z = -camera->viewPos.z;
    
    camera->viewUp.x = 0;
    camera->viewUp.y = 0;
    camera->viewUp.z = 1;
    
    cam->x = 0;
    cam->y = 0;
    
    *shapeSize = 40.0f; // max radius of of objects
    
    updateCamera(camera,shapeSize);
}

void easyinit(recCamera* camera,recVec* cam,GLfloat* shapeSize,int* frame,int* curr_pos,int* display){
    resetCamera(camera,cam,shapeSize);
    *frame = 0;
    *curr_pos = 0;
    *display = false;
    
    glEnable(GL_LINE_SMOOTH);
    
    //obj = stl("/Users/crinq/Dropbox/maschinen/knuth/pumpe.stl");
}

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
