#include "Player.h"

/*
	Creates the player, takes in a position, rotation and rendering engine
*/
Player::Player(glm::vec3 pos, glm::vec3 rot, XyEngine* engine)
{
	this->m_pEngine = engine;
	this->m_pos = pos;
	this->m_rot = rot;
	this->m_Camera = new Camera(m_pEngine);
	
	this->m_Camera->SetCameraPosition(this->m_pos);
	this->m_Camera->SetCameraPosition(this->m_rot);
}

/*
	Sets the players position
*/
void Player::SetPosition(glm::vec3 pos)
{
	this->m_pos = pos;
	this->m_Camera->SetCameraPosition(this->m_pos);
}

/*
	Sets the players rotation
*/
void Player::SetRotation(glm::vec3 rot)
{
	this->m_rot = rot;
	this->m_Camera->SetCameraPosition(this->m_rot);
}

/*
	Returns the players position
*/
glm::vec3 Player::GetPosition()
{
	return this->m_pos;
}

/*
	Returns the players rotation
*/
glm::vec3 Player::GetRotation()
{
	return this->m_rot;
}

/*
	Updates the Players position
*/
void Player::Update(bool mousein)
{
	this->m_pos = this->m_Camera->GetCameraPosition();
	this->m_rot = this->m_Camera->GetCameraRotation();

	this->m_Camera->SetCameraPosition(this->m_pos);
	this->m_Camera->SetCameraRotation(this->m_rot);

	this->m_Camera->UpdateControls(0.2f, 0.05f, mousein);
	this->m_Camera->UpdateCamera();
}
