#include "easygl.h"
#include "interpolator.h"
#include "gcode.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float speed = 5.0f; // movement speed
const float sensitivity = 0.01f; // mouse sensitivity

easygl renderer;
double time;
bool w = false, s = false, a = false, d = false, q = false, e = false;
glm::vec3 movement;
int drag;
glm::ivec2 mouse, lastMouse;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_REPEAT)
		return;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	else if(key == GLFW_KEY_W)
		w = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_S)
		s = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_A)
		a = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_D)
		d = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_Q)
		q = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_E)
		e = action == GLFW_PRESS ? true : false;

	movement.x = d - a;
	movement.y = q - e;
	movement.z = s - w;
}

static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
        drag = true;
    else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
        drag = false;
}

static void mousepos_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse = glm::ivec2(xpos, ypos);
	if(drag)
	{
		glm::ivec2 diff = lastMouse - mouse;
		renderer.orientation = renderer.orientation * glm::quat(glm::vec3(diff.y, diff.x, 0) * sensitivity);
	}
	lastMouse = mouse;
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
    	double newTime = glfwGetTime();
    	double delta = newTime - time;
    	time = newTime;

		// update camera
    	glm::vec3 direction = glm::rotate(renderer.orientation, glm::vec3(0, 0, -1));
    	glm::vec3 right = glm::cross(renderer.up, direction);
    	glm::vec3 up = glm::cross(direction, right);
		renderer.position -= (movement.z * direction + movement.x * right + movement.y * up) * speed * (float)delta;
    	renderer.target = renderer.position + direction;

        glfwGetFramebufferSize(window, &renderer.viewportSize.x, &renderer.viewportSize.y);
        renderer.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
