#pragma once
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

static const float YAW = -90.0f;
static const float PITCH = 0.0f;
static const float SPEED = 2.5f;
static const float SENSITIVITY = 0.1f;
static const float SCROLLRATE = 3.0f;
static const float ZOOM = 45.0f;

class ViewCamera {
public:
	// constructor with vectors
	ViewCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// constructor with scalar values
	ViewCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	ViewCamera(glm::vec3 position, glm::vec3 lookat, glm::vec3 up) :MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Front = glm::normalize(lookat - position);
		//printf("Front.y:%f \n", Front.y);
		Position = position;
		WorldUp = up;
		Pitch = glm::degrees(glm::asin(Front.y));
		Yaw = glm::degrees(glm::acos(Front.x / glm::cos(glm::radians(Pitch)))) * Front.x / glm::abs(Front.x);
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
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		updateCameraVectors();
	}
	void ProcessRightMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= SENSITIVITY * SENSITIVITY;
		yoffset *= SENSITIVITY * SENSITIVITY;

		Position += Right * xoffset + Up * yoffset;
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Position += Front * SENSITIVITY * yoffset * SCROLLRATE;
	}

private:
	// Default camera values

	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	void updateCameraVectors() {
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));

		//printf("Yaw:%f Pitch:%f\n", Yaw, Pitch);
	}

};
