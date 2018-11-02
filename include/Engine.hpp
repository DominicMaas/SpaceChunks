#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "SDL2/SDL.h"
#include <GL/glew.h>
#include <GL/gl.h>
#undef main
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
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <sstream>


#ifndef M_PI
#define M_PI = 3.14159265358979
#endif

#define CHUNK_X 16
#define CHUNK_Y 64
#define CHUNK_Z 16

class Engine
{
private:
	void(*m_InitFunc)();
	void(*m_RenderFunc)();
	void(*m_PhysicsFunc)();
	void(*m_InputFunc)(SDL_Event event);
	float m_FrameRate;

	SDL_Window* m_Window;
	SDL_Thread* m_pPhysicsThread;
	SDL_GLContext glcontext;
	SDL_Event event;
	bool m_running;
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

	static int StaticPhysicsThread(void *ptr);

	int PhysicsThread();

	const char* XyEngine_Version = "XyEngine 0.3.6";

public:
	Engine(void(*initFunc)(), void(*renderFunc)(), void(*inputFunc)(SDL_Event event), void(*physicsFunc)());

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

	const char* GetXyEngineVersion()
	{
		return XyEngine_Version;
	}

	void GenSeed()
	{
		srand(time(0));
		seed = rand();
	}
	void Set3D()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(70.0f, (GLdouble)m_windowWidth / (GLdouble)m_windowHeight, 0.01, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
	}

	void Set2D()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, m_windowWidth, m_windowHeight, 0, 0.01, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
	}

	void TranslateWorldMatrix(float x, float y, float z)
	{
		glTranslatef(x, y, z);
	}

	void TranslateWorldMatrix(glm::vec3 pos)
	{
		glTranslatef(pos.x, pos.y, pos.z);
	}

	void PushMatrix()
	{
		glPushMatrix();
	}

	void PopMatrix()
	{
		glPopMatrix();
	}

	void RotateWorldMatrix_X(float angle)
	{
		glRotatef(angle, 1, 0, 0);
	}

	void RotateWorldMatrix_Y(float angle)
	{
		glRotatef(angle, 0, 1, 0);
	}

	void RotateWorldMatrix_Z(float angle)
	{
		glRotatef(angle, 0, 0, 1);
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

	SDL_Window* GetWindow()
	{
		return m_Window;
	}

	SDL_Event GetEvent()
	{
		return event;
	}

	bool IsMouseIn()
	{
		return mousein;
	}

	bool Running()
	{
		return m_running;
	}

	void ClearScreen(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	void ClearScreen(GLbitfield mask)
	{
		glClear(mask);
	}

	unsigned int CreateTexture(float w, float h, bool isDepth = false)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, (!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT), (GLsizei)w, (GLsizei)h, 0, isDepth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_FLOAT, NULL);;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		int i = 0;
		i = glGetError();
		if (i != 0)
		{
			std::cout << "Error happened while creating the texture: " << i << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		return textureID;
	}

	void RenderText(float x, float y, const std::string message);

	void BindTexture(GLuint &tex, int i)
	{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE + i);
		glBindTexture(GL_TEXTURE_2D, tex);
	}

	void UnbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	void DisposeTexture(GLuint &tex)
	{
		glDeleteTextures(1, &tex);
	}

	void LoadTexture(GLuint &tex, const char* filename)
	{
		glGenTextures(1, &tex);

		SDL_Surface* img = IMG_Load_RW(SDL_RWFromFile(filename, "rb"), 1);

		if (img == NULL)
		{
			printf("Unable to load Image: %s\n", SDL_GetError());
			tex = NULL;
		}

		else
		{
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
			SDL_FreeSurface(img);
		}
	}
};

#endif // ENGINE_HPP