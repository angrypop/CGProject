#pragma once
#include "Camera.h"
#include "ViewHeader.h"

class Interaction
{
	static void UpdateData();
public:
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static GLfloat ReadXoffset();
	static GLfloat ReadYoffset();


	static bool screenShotFlag;
	static bool mouseReverseFlag;
	static ViewCamera camera;
	static glm::vec3 ObjPos;
	static glm::vec3 ObjVel;
	static bool key_space_pressed;
	static bool key_w_pressed;
	static bool key_s_pressed;
	static bool key_a_pressed;
	static bool key_d_pressed;
	static bool key_y_flag;
	static bool key_r_pressed;
	static bool left_button_pressed;
	static bool right_button_pressed;
	static GLfloat yaw;
	static GLfloat pitch;
	static glm::vec3 front;

private:
	static float mouse_sensitivity;
	static float lastX;
	static float lastY;
	static GLfloat xoffset;
	static GLfloat yoffset;
	static bool left_button_pressed_just;
	static bool right_button_pressed_just;
};
