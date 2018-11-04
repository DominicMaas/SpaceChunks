﻿#pragma once

#include "GL/glew.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#define GLM_FORCE_RADIANS
#include "GLM/glm.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm> 
#include <time.h>  
#include <vector>
#include <fstream>
#include <string>
#include <Time.hpp>
#include <thread>
#include <sstream>

#ifndef M_PI
#define M_PI = 3.14159265358979
#endif

#define CHUNK_X 16
#define CHUNK_Y 64
#define CHUNK_Z 16

class Engine {




private:
	// Functions that run on the engine
	void(*m_InitFunc)();
	void(*m_RenderFunc)();
	void(*m_PhysicsFunc)();
	void(*m_InputFunc)(sf::Event event);
	
	// SFML variables
	
	sf::Event _event;

	// Thread for the physics engine
	sf::Thread _physicsThread;

	bool _isRunning;

	float m_FrameRate;

	bool mousein;

	float m_frameTime;
	float frames = 0;
	float fpsRate;

	float m_physicsFrameTime = 1.0f / 60.0f;
	float pysicsFrames = 0;
	float physicsRate;

	int seed;

	int m_windowWidth;
	int m_windowHeight;

	int m_middleWidth;
	int m_middleHeight;

	void PhysicsThread();

	const char* XyEngine_Version = "XyEngine 0.3.6";

public:
	sf::Window _window;

	Engine(void(*initFunc)(), void(*renderFunc)(), void(*inputFunc)(sf::Event event), void(*physicsFunc)());

	int CreateWindow(int width, int height, char* title, float frameRate);
	int ReturnWithError(std::string err);
	void Log(const char* text);


	int GetWindowWidth()
	{
		return m_windowWidth;
	}

	int GetWindowHeight()
	{
		return m_windowHeight;
	}

	int GetWindowMiddleWidth()
	{
		return m_middleWidth;
	}

	int GetWindowMiddleHeight()
	{
		return m_middleHeight;
	}

	~Engine();

	int GetSeed()
	{
		return seed;
	}

	void GenSeed()
	{
		srand(time(0));
		seed = rand();
	}

	std::string ConvertIntToString(int num)
	{
		std::string str;
		std::ostringstream con;

		con << num;

		str = con.str();

		return str;
	}

	const char* ConvertDouble(double num)
	{
		std::string str;
		std::ostringstream con;

		con << num;

		str = con.str();

		return str.c_str();
	}

	float GetFPS()
	{
		return fpsRate;
	}

	float GetPhysicsFPS()
	{
		return physicsRate;
	}

	sf::Window* GetWindow()
	{
		return &_window;
	}

	sf::Event* GetEvent()
	{
		return &_event;
	}

	bool IsMouseIn()
	{
		return mousein;
	}

	bool Running()
	{
		return _isRunning;
	}

	void ClearScreen(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	void ClearScreen(GLbitfield mask)
	{
		glClear(mask);
	}

	void RenderText(float x, float y, const std::string message);
};

#endif // ENGINE_HPP