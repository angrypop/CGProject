
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GlobalData.h"
enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

static const float YAW = 0.0f;
static const float PITCH = 0.0f;
static const float SPEED = 2.5f;
static const float SENSITIVITY = 0.05f;
static const float SCROLLRATE = 3.0f;
static const float ZOOM = 45.0f;
static const float RADIAN_TO_ANGLE = 180.0f / 3.1416f;
static const float FOLLOW_SPEED = 0.01f; // must be in (0, 1)

class ViewCamera {
public:
	void updateGlobalData()
	{
		GlobalDataPool::SetData<glm::mat4>("cameraView", this->GetViewMatrix());
		GlobalDataPool::SetData<glm::vec3>("cameraPosition", this->GetViewPosition());
	}

	// constructor with vectors
	ViewCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		updateCameraVectors();
	}
	// constructor with scalar values
	ViewCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		updateCameraVectors();
	}
	ViewCamera(glm::vec3 position, glm::vec3 lookat, glm::vec3 up) :MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Front = glm::normalize(lookat - position);
		//printf("Front.y:%f \n", Front.y);
		Position = position;
		WorldUp = up;
		updateCameraVectors();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	glm::vec3 GetViewPosition() {
		return Position;
	}

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == CameraMovement::FORWARD)
			Position += Front * velocity;
		if (direction == CameraMovement::BACKWARD)
			Position -= Front * velocity;
		if (direction == CameraMovement::LEFT)
			Position -= Right * velocity;
		if (direction == CameraMovement::RIGHT)
			Position += Right * velocity;
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessLeftMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		static const float pixelPerAngle = 50;
		glm::mat4 m = glm::rotate(glm::mat4(1.0f), yoffset * SENSITIVITY, Right);
		m = glm::rotate(m, xoffset * SENSITIVITY, Up);
		SetFrontDir(m * glm::vec4({ Front[0], Front[1], Front[2], 1 }), true);
		this->updateGlobalData();
	}

	void ProcessRightMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		this->updateGlobalData();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Position += Front * SENSITIVITY * yoffset * SCROLLRATE;
		this->updateGlobalData();
	}

	void ProcessMouseMovement(float yoffset)
	{
		this->updateGlobalData();
	}

	void SetPosition(glm::vec3 in_position)
	{
		Position = in_position;
		this->updateGlobalData();
	}

	void SetFrontDir(glm::vec3 in_front, bool hard = false)
	{
		if (hard) {
			Front = in_front;
		}
		else {
			// smooth transition
			Front = (1 - FOLLOW_SPEED) * Front + FOLLOW_SPEED * in_front;
		}
		updateCameraVectors();
		this->updateGlobalData();
	}

	void SetWorldUpDir(glm::vec3 in_up)
	{
		WorldUp = in_up;
		updateCameraVectors();
		this->updateGlobalData();
	}

	void Move(CameraMovement dir, GLfloat dist)
	{
		if (dir == CameraMovement::FORWARD)
			Position += Front * dist;
		if (dir == CameraMovement::BACKWARD)
			Position -= Front * dist;
		if (dir == CameraMovement::LEFT)
			Position -= Right * dist;
		if (dir == CameraMovement::RIGHT)
			Position += Right * dist;
		this->updateGlobalData();
	}

private:
	// Default camera values

	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	void updateCameraVectors() {
		Front = glm::normalize(Front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
		/*
		static int cnt = 0;
		if (cnt++ % 1000) return;
		printf("Yaw:%f Pitch:%f\n", Yaw, Pitch);
		printf("WorldUp:%f %f %f\n", WorldUp[0], WorldUp[1], WorldUp[2]);
		*/
	}

};
