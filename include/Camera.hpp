#pragma once

#include "Engine.hpp"

enum Motion {
	Front,
	Back,
	Left,
	Right
};

class Camera {
public:
	Camera(Engine* engine);

	glm::vec3 position();
	void setPosition(glm::vec3 position);

	glm::vec3 rotation();
	void setRotation(glm::vec3 rotation);

	void updateControls(float moveSpeed, float mouseSpeed, bool mouseIn);
	void updateCamera();

	void LockMotion(Motion axis) 
	{
		switch (axis) {
		case Motion::Front:
			m_lockFront = true;
			break;
		case Motion::Back:
			m_lockBack = true;
			break;
		case Motion::Left:
			m_lockLeft = true;
			break;
		case Motion::Right:
			m_lockRight = true;
			break;
		}
	}

	void UnlockMotion(int axis) 
	{
		switch (axis) {
		case Motion::Front:
			m_lockFront = false;
			break;
		case Motion::Back:
			m_lockBack = false;
			break;
		case Motion::Left:
			m_lockLeft = false;
			break;
		case Motion::Right:
			m_lockRight = false;
			break;
		}
	}

private:
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	
	Engine* m_engine;

	sf::Vector2i m_previousMousePosition;

	bool m_lockFront = false;
	bool m_lockBack = false; 
	bool m_lockLeft = false;
	bool m_lockRight = false;

	void lockCamera();
	void moveCamera(float distance, float direction);
	void moveCameraUp(float distance, float direction);
};