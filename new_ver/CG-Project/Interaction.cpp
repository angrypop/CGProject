
#include "Interaction.h"


bool Interaction::mouseReverseFlag = true;
float Interaction::mouse_sensitivity = 0.10f;
bool Interaction::screenShotFlag = false;

constexpr GLfloat groundSize = 15.0f;
ViewCamera Interaction::camera = ViewCamera(glm::vec3(40.0f, 40.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::vec3 Interaction::ObjPos = glm::vec3(0.0f, groundSize * 3 / 2, groundSize * 2);
glm::vec3 Interaction::ObjVel = glm::vec3(0.0f);
bool Interaction::key_space_pressed = false;
bool Interaction::key_w_pressed = false;
bool Interaction::key_s_pressed = false;
bool Interaction::key_a_pressed = false;
bool Interaction::key_d_pressed = false;
bool Interaction::key_y_flag = false;
bool Interaction::key_p_flag = false;
GLfloat Interaction::yaw = 0;
GLfloat Interaction::pitch = 0;
GLfloat Interaction::xoffset = 0;
GLfloat Interaction::yoffset = 0;
glm::vec3 Interaction::front = glm::vec3(0.0f);

float Interaction::lastX = 0.0f;
float Interaction::lastY = 0.0f;
bool Interaction::left_button_pressed = false;
bool Interaction::left_button_pressed_just = false;
bool Interaction::right_button_pressed = false;
bool Interaction::right_button_pressed_just = false;



void Interaction::UpdateData()
{
	GlobalDataPool::SetData<glm::vec3>("uniObjPos", ObjPos);
	GlobalDataPool::SetData<glm::vec3>("uniObjVel", ObjVel);
	// std::cout << "obj pos = " << ObjPos.x << " " << ObjPos.y << " " << ObjPos.z << std::endl;
	// std::cout << "obj vel = " << ObjVel.x << " " << ObjVel.y << " " << ObjVel.z << std::endl;
}

void Interaction::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	float xposf = (float)xpos;
	float yposf = (float)ypos;
	float tmp_xoffset = (xposf - lastX) * mouse_sensitivity;
	float tmp_yoffset = (yposf - lastY) * mouse_sensitivity;
	lastX = xposf;
	lastY = yposf;
	if (left_button_pressed) {
		// modify camera view dir
		if (left_button_pressed_just) {
			lastX = xposf;
			lastY = yposf;
			left_button_pressed_just = 0;
		}
		if (mouseReverseFlag)
			camera.ProcessLeftMouseMovement(tmp_xoffset, tmp_yoffset);
		else
			camera.ProcessLeftMouseMovement(-tmp_xoffset, -tmp_yoffset);
	}
	else if (right_button_pressed) {
	}
	else {
		// modify game obj dir
		// will be consumed by the loop in ViewWorld.cpp
		xoffset = tmp_xoffset;
		yoffset = tmp_yoffset;
		// below: not used
		yaw += xoffset;
		pitch += yoffset;
		glm::vec3 new_front;
		new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		new_front.y = sin(glm::radians(pitch));
		new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = new_front;
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
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		mouseReverseFlag = !mouseReverseFlag;
	}

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)	// screen shot
	{
		std::cout << "F1 Pressed" << std::endl;
		screenShotFlag = true;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		key_space_pressed = true;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		key_space_pressed = false;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		key_w_pressed = true;
		printf("Key W\n");
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		key_w_pressed = false;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		key_a_pressed = true;
		printf("Key A\n");
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		key_a_pressed = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		key_s_pressed = true;
		printf("Key S\n");
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		key_s_pressed = false;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		key_d_pressed = true;
		printf("Key D\n");
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		key_d_pressed = false;
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		camera.SetWorldUpDir({ 0, 1, 0 });
		key_y_flag = !key_y_flag;
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		camera.SetWorldUpDir({ 0, 1, 0 });
		key_p_flag = !key_p_flag;
	}
	if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
		//camera.ProcessKeyboard(CameraMovement::FORWARD, 1);
		ObjPos += glm::vec3(-0.05, 0, 0);
		ObjVel = glm::vec3(-1, 0, 0);
	}
	if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
		//camera.ProcessKeyboard(CameraMovement::LEFT, 1);
		ObjPos += glm::vec3(0, 0, 0.05);
		ObjVel = glm::vec3(0, 0, 1);
	}
	if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
		//camera.ProcessKeyboard(CameraMovement::BACKWARD, 1);
		ObjPos += glm::vec3(0.05, 0, 0);
		ObjVel = glm::vec3(1, 0, 0);
	}
	if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
		//camera.ProcessKeyboard(CameraMovement::RIGHT, 1);
		ObjPos += glm::vec3(0, 0, -0.05);
		ObjVel = glm::vec3(0, 0, -1);
	}

	UpdateData();
}

GLfloat Interaction::ReadXoffset()
{
	GLfloat ret = xoffset;
	xoffset = 0;
	return ret;
}

GLfloat Interaction::ReadYoffset()
{
	GLfloat ret = yoffset;
	yoffset = 0;
	return ret;
}

