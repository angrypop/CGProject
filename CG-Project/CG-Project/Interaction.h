#pragma once
#include "Camera.h"
#include "ViewBase.h"

class Interaction
{
public:
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static ViewCamera camera;
	static glm::vec3 ObjPos;
	static glm::vec3 ObjVel;
	static bool spaceFlag;

private:
	static float lastX;
	static float lastY;
	static int left_button_pressed;
	static int left_button_pressed_just;
	static int right_button_pressed;
	static int right_button_pressed_just;
};
