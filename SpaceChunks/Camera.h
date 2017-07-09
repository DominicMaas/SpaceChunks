#pragma once
#include "XyEngine.h"

const int MOTION_FRONT = 0;
const int MOTION_BACK = 1;
const int MOTION_LEFT = 2;
const int MOTION_RIGHT = 3;

class Camera
{
private:
	glm::vec3 m_CamPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 m_CamRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	XyEngine* m_Engine;

	void LockCamera();
	void MoveCamera(float distance, float direction);
	void MoveCameraUp(float distance, float direction);

	bool m_pLockFront = false, m_pLockBack = false, m_pLockLeft = false, m_pLockRight = false;

public:

	Camera(XyEngine* engine);

	glm::vec3 GetCameraPosition();
	void SetCameraPosition(glm::vec3 pos);

	glm::vec3 GetCameraRotation();
	void SetCameraRotation(glm::vec3 pos);

	void UpdateControls(float moveSpeed, float mouseSpeed, bool mouseIn);
	void UpdateCamera();

	void LockMotion(int axis) {
		switch (axis)
		{
		case MOTION_FRONT:
			m_pLockFront = true;
			break;
		case MOTION_BACK:
			m_pLockBack = true;
			break;
		case MOTION_LEFT:
			m_pLockLeft = true;
			break;
		case MOTION_RIGHT:
			m_pLockRight = true;
			break;
		}
	}

	void UnlockMotion(int axis) {
		switch (axis)
		{
		case MOTION_FRONT:
			m_pLockFront = false;
			break;
		case MOTION_BACK:
			m_pLockBack = false;
			break;
		case MOTION_LEFT:
			m_pLockLeft = false;
			break;
		case MOTION_RIGHT:
			m_pLockRight = false;
			break;
		}
	}
};


