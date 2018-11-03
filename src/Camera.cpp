#include "Camera.hpp"

Camera::Camera(Engine* engine)
{
	this->m_Engine = engine;

	_previousMousePosition = sf::Vector2i(engine->GetWindowWidth() / 2, engine->GetWindowHeight() / 2);
}

glm::vec3 Camera::GetCameraPosition()
{
	return m_CamPosition;
}

void Camera::SetCameraPosition(glm::vec3 pos)
{
	this->m_CamPosition = pos;
}

glm::vec3 Camera::GetCameraRotation()
{
	return m_CamRotation;
}

void Camera::SetCameraRotation(glm::vec3 pos)
{
	this->m_CamRotation = pos;
}

void Camera::LockCamera()
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

void Camera::MoveCamera(float distance, float direction)
{
	float rad = (m_CamRotation.y + direction) * M_PI / 180.0;

	m_CamPosition.x -= sin(rad) * distance;
	m_CamPosition.z -= cos(rad) * distance;
}

void Camera::MoveCameraUp(float distance, float direction)
{
	float rad = (m_CamRotation.y + direction) * M_PI / 180.0;
	m_CamPosition.y += (sin(rad)*distance) + 1.0f;
}

void Camera::UpdateControls(float moveSpeed, float mouseSpeed, bool mouseIn)
{
	if (mouseIn)
	{
		// Get the current mouse position in the window
		sf::Vector2i mousePos = sf::Mouse::getPosition(m_Engine->_window);

		// Get the x and y offset, reversed Y since y-coordinates range from bottom to top
		float xOffset = mousePos.x - _previousMousePosition.x;
		float yOffset = _previousMousePosition.y - mousePos.y;

		// Update the previous mouse position
		_previousMousePosition = mousePos;

		float sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		// Apply the camera rotation
		m_CamRotation.y += yOffset; //0.05f*xOffset; //(m_Engine->GetWindowMiddleWidth() - xOffset);
		m_CamRotation.x += xOffset; //0.05f*yOffset; //(m_Engine->GetWindowMiddleHeight() - yOffset);
		LockCamera();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if (!m_pLockFront)
				MoveCamera(moveSpeed, 0.0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (!m_pLockBack)
				MoveCamera(moveSpeed, 180.0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (!m_pLockLeft)
				MoveCamera(moveSpeed, 90.0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (!m_pLockRight)
				MoveCamera(moveSpeed, 270);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			m_CamPosition.y += moveSpeed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			m_CamPosition.y -= moveSpeed;
		}
	}
	m_Engine->RotateWorldMatrix_X(cos(glm::radians(m_CamRotation.y)) * cos(glm::radians(m_CamRotation.x))); //(-m_CamRotation.x);
	m_Engine->RotateWorldMatrix_Y(sin(glm::radians(m_CamRotation.y)));  //(-m_CamRotation.y); // 
	m_Engine->RotateWorldMatrix_Z(-m_CamRotation.z);
}

void Camera::UpdateCamera()
{
	m_Engine->TranslateWorldMatrix(-m_CamPosition.x, -m_CamPosition.y - 2.0f, -m_CamPosition.z);
}