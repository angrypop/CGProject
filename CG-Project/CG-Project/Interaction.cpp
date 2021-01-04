#include "Interaction.h"

ViewCamera Interaction::camera = ViewCamera(glm::vec3(40.0f, 40.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::vec3 Interaction::ObjPos = glm::vec3(0.0f);
glm::vec3 Interaction::ObjVel = glm::vec3(0.0f);
bool Interaction::spaceFlag = false;
bool Interaction::key_w_pressed = false;
bool Interaction::key_s_pressed = false;
bool Interaction::key_a_pressed = false;
bool Interaction::key_d_pressed = false;

float Interaction::lastX = 0.0f;
float Interaction::lastY = 0.0f;
int Interaction::left_button_pressed = 0;
int Interaction::left_button_pressed_just = 0;
int Interaction::right_button_pressed = 0;
int Interaction::right_button_pressed_just = 0;


void Interaction::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	float xposf = (float)xpos;
	float yposf = (float)ypos;
	if (left_button_pressed) {
		if (left_button_pressed_just) {
			lastX = xposf;
			lastY = yposf;
			left_button_pressed_just = 0;
		}

		float xoffset = xposf - lastX;
		float yoffset = lastY - yposf;
		lastX = xposf;
		lastY = yposf;

		float sensitivity = 0.5;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera.ProcessLeftMouseMovement(-xoffset, -yoffset);
	}

	if (right_button_pressed) {
		if (right_button_pressed_just) {
			lastX = xposf;
			lastY = yposf;
			right_button_pressed_just = 0;
		}

		float xoffset = xposf - lastX;
		float yoffset = lastY - yposf;
		lastX = xposf;
		lastY = yposf;

		float sensitivity = 0.5;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera.ProcessRightMouseMovement(-xoffset, -yoffset);
	}
}

void Interaction::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		left_button_pressed = 1;
		left_button_pressed_just = 1;
		printf("GET LEFT BUTTON PRESS\n");
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		left_button_pressed = 0;
		printf("GET LEFT BUTTON RELEASE\n");
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		right_button_pressed = 1;
		right_button_pressed_just = 1;
		printf("GET RIGHT BUTTON PRESS\n");
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		right_button_pressed = 0;
		printf("GET RIGHT BUTTON RELEASE\n");
	}
}

void Interaction::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

void Interaction::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		spaceFlag = !spaceFlag;
		printf("Key Space\n");
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		ObjPos += glm::vec3(-0.05, 0, 0);
		ObjVel = glm::vec3(-1, 0, 0);
		key_w_pressed = true;
		printf("Key W\n");
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		key_w_pressed = false;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		ObjPos += glm::vec3(0, 0, 0.05);
		ObjVel = glm::vec3(0, 0, 1);
		key_a_pressed = true;
		printf("Key A\n");
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		key_a_pressed = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		ObjPos += glm::vec3(0.05, 0, 0);
		ObjVel = glm::vec3(1, 0, 0);
		key_s_pressed = true;
		printf("Key S\n");
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		key_s_pressed = false;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		ObjPos += glm::vec3(0, 0, -0.05);
		ObjVel = glm::vec3(0, 0, -1);
		key_d_pressed = true;
		printf("Key D\n");
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		key_d_pressed = false;
	}
}
