#include "easygl.h"
#include "interpolator.h"
#include "gcode.h"
#include <GLFW/glfw3.h>

easygl renderer;
int drag;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
        drag = true;
    if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
        drag = false;
    
    fflush(stdout);
}

static void mousepos_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(drag){
        printf("%f %f\n",xpos,ypos);
    }
    fflush(stdout);
}

static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    double deltaAperture = yoffset * -renderer.fieldOfView / 200.0;
    renderer.fieldOfView += deltaAperture;
    if (renderer.fieldOfView < 0.1) // do not let aperture <= 0.1
        renderer.fieldOfView = 0.1;
    if (renderer.fieldOfView > 179.9) // do not let aperture >= 180
        renderer.fieldOfView = 179.9;
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "KinSim", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window,mousebutton_callback);
    glfwSetCursorPosCallback(window,mousepos_callback);
    
    renderer.currentPath = gcode("gcode.ngc");
    interpol(renderer.currentPath);
    renderer.robotState = &renderer.currentPath->pos;
    renderer.init();
    
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &renderer.viewportSize.x, &renderer.viewportSize.y);
        renderer.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
