#include "easygl.h"
#include "interpolator.h"
#include "gcode.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{
    int width, height;
    
    recCamera camera;
    recVec cam;
    int curr_pos;
    int frame;
    int display;
    GLfloat shapeSize;
    GLfloat j1,j2,j3;
    path* currentPath;
    outpath p;
    
    currentPath = gcode("gcode.ngc");
    p = interpol(currentPath);
    display = true;
    
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    glfwGetFramebufferSize(window, &width, &height);
    camera.viewHeight = height;
    camera.viewWidth = width;
    
    easyinit(&camera,&cam,&shapeSize,&frame,&curr_pos,&display);
    
    while (!glfwWindowShouldClose(window))
    {
        /*
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
        */
        glfwGetFramebufferSize(window, &width, &height);
        camera.viewHeight = height;
        camera.viewWidth = width;
        easydraw(&cam,&display,&curr_pos,&j1,&j2,&j3,&p,currentPath);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
