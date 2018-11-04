#include "Camera.hpp"

Camera::Camera(Engine* engine)
{
	this->m_engine = engine;
	this->m_previousMousePosition = sf::Vector2i(engine->GetWindowWidth() / 2, engine->GetWindowHeight() / 2);
}

glm::vec3 Camera::position()
{
	return m_position;
}

void Camera::setPosition(glm::vec3 position)
{
	this->m_position = position;
}

glm::vec3 Camera::rotation()
{
	return m_rotation;
}

void Camera::setRotation(glm::vec3 rotation)
{
	this->m_rotation = rotation;
}

void Camera::lockCamera()
{
	//if (m_CamRotation.y < 0.0)
	//	m_CamRotation.y = 360;

	//if (m_CamRotation.y > 360.0)
	//	m_CamRotation.y = 360;

	//if (m_CamRotation.z < 0.0)
	//	m_CamRotation.z = 360;

	//if (m_CamRotation.z > 360.0)
	//	m_CamRotation.z = 360;

	//if (m_CamRotation.x > 90)
	//	m_CamRotation.x = 90;

	//if (m_CamRotation.x < -90)
	//	m_CamRotation.x = -90;
}

void Camera::moveCamera(float distance, float direction)
{
	float rad = (m_rotation.y + direction) * M_PI / 180.0;

	m_position.x -= sin(rad) * distance;
	m_position.z -= cos(rad) * distance;
}

void Camera::moveCameraUp(float distance, float direction)
{
	float rad = (m_rotation.y + direction) * M_PI / 180.0;
	m_position.y += (sin(rad)*distance) + 1.0f;
}

void Camera::updateControls(float moveSpeed, float mouseSpeed, bool mouseIn)
{
	if (mouseIn) {
		// Get the current mouse position in the window
		sf::Vector2i mousePos = sf::Mouse::getPosition(m_engine->_window);

		// Get the x and y offset, reversed Y since y-coordinates range from bottom to top
		float xOffset = mousePos.x - m_previousMousePosition.x;
		float yOffset = m_previousMousePosition.y - mousePos.y;

		// Update the previous mouse position
		m_previousMousePosition = mousePos;

		float sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		// Apply the camera rotation
		m_rotation.y += yOffset; //0.05f*xOffset; //(m_Engine->GetWindowMiddleWidth() - xOffset);
		m_rotation.x += xOffset; //0.05f*yOffset; //(m_Engine->GetWindowMiddleHeight() - yOffset);
		lockCamera();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			moveCamera(moveSpeed, 0.0);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			moveCamera(moveSpeed, 180.0);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			moveCamera(moveSpeed, 90.0);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			moveCamera(moveSpeed, 270);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			m_position.y += moveSpeed;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			m_position.y -= moveSpeed;
		}
	}

	m_engine->RotateWorldMatrix_X(-m_rotation.x);
	m_engine->RotateWorldMatrix_Y(-m_rotation.y);
	m_engine->RotateWorldMatrix_Z(-m_rotation.z);
}

void Camera::updateCamera()
{
	m_engine->TranslateWorldMatrix(-m_position.x, -m_position.y - 2.0f, -m_position.z);
}