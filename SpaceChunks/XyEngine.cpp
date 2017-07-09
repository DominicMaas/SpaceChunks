#include "XyEngine.h"

XyEngine::XyEngine(void(*initFunc)(), void(*renderFunc)(), void(*inputFunc)(SDL_Event event), void(*physicsFunc)())
{
	this->m_InitFunc = initFunc;
	this->m_RenderFunc = renderFunc;
	this->m_InputFunc = inputFunc;
	this->m_PhysicsFunc = physicsFunc;
}

int XyEngine::StaticPhysicsThread(void *ptr)
{
	return ((XyEngine*)ptr)->PhysicsThread();
}

int XyEngine::PhysicsThread()
{
	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	pysicsFrames = 0;

	while (m_running)
	{
		bool physicsUpdate = false;

		double startTime = Time::GetTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0)
		{
			physicsRate = pysicsFrames;
			pysicsFrames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_physicsFrameTime)
		{
			physicsUpdate = true;
			unprocessedTime -= m_physicsFrameTime;
		}

		if (physicsUpdate)
		{
			m_PhysicsFunc();
			pysicsFrames++;
		}
		else{
			SDL_Delay(1);
		}

	}

	return 0;
}

int XyEngine::CreateWindow(int width, int height, char* title, float frameRate)
{
	double StartTime = Time::GetTime();
	Log("XyEngine is Loading...");
	
	this->m_windowWidth = width;
	this->m_windowHeight = height;

	this->m_middleWidth = m_windowWidth / 2;
	this->m_middleHeight = m_windowHeight / 2;

	this->m_FrameRate = frameRate;
	this->m_frameTime = 1.0 / this->m_FrameRate;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return ReturnWithError("SDL Failed To Init!");

	m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

	if (m_Window == NULL)
	{
		SDL_Quit();
		return ReturnWithError("SDL Failed To Create the Window!");
	}

	glcontext = SDL_GL_CreateContext(m_Window);

	if (glewInit() != GLEW_OK)
		return ReturnWithError("GLEW Failed to Init!");

	if (TTF_Init() != 0)
		return ReturnWithError("TTF Failed to Init!");

	if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
		return ReturnWithError("IMG Failed to Init!");

	m_running = true;

	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	frames = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE_ARB);

	m_InitFunc();

	m_pPhysicsThread = SDL_CreateThread(StaticPhysicsThread, "PhysicsThread", this);

	if (NULL == m_pPhysicsThread) {
		ReturnWithError("Physics Thread Failed to create");
	}
	else
	{
		Log("Physics Thread Created");
	}

	double EndTime = Time::GetTime() - StartTime;
	std::cout << "Time Taken to load XyEngine: " << EndTime << std::endl;

	SDL_Event event;

	while (m_running)
	{
		bool render = false;

		double startTime = Time::GetTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0)
		{
			fpsRate = frames;
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime)
		{
			render = true;

			while (SDL_PollEvent(&event))
			{
				m_InputFunc(event);

				switch (event.type)
				{

				case SDL_QUIT:
					m_running = false;
					break;

				case SDL_MOUSEBUTTONDOWN:
					mousein = true;
					SDL_ShowCursor(SDL_DISABLE);
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						mousein = false;
						SDL_ShowCursor(SDL_ENABLE);
						break;
					}
				}
			}

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			if (mousein)
				SDL_WarpMouseInWindow(m_Window, m_middleWidth, m_middleHeight);

			m_RenderFunc();
			SDL_GL_SwapWindow(m_Window);
			frames++;
		}
		else
		{
			SDL_Delay(1);
		}
	}


	return EXIT_SUCCESS;
}

int XyEngine::ReturnWithError(std::string err)
{
	Log(err.c_str());
	return EXIT_FAILURE;
}

void XyEngine::Log(const char* text)
{
	std::cout << "[XYEngine] " << text << std::endl;
}

void XyEngine::RenderText(float x, float y, const std::string message)
{

	SDL_Color color = { 255, 255, 255 };

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluOrtho2D(0, m_windowWidth, m_windowHeight, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	TTF_Font * m_font = TTF_OpenFont("fonts/font3.ttf", 14);
	SDL_Surface * sFont = TTF_RenderText_Blended(m_font, message.c_str(), color);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(x + sFont->w, y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(x + sFont->w, y + sFont->h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + sFont->h);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);

	glDeleteTextures(1, &texture);
	TTF_CloseFont(m_font);
	SDL_FreeSurface(sFont);

}

XyEngine::~XyEngine()
{
	if (m_Window != NULL)
	{
		printf("[CORE] XyEngine is shutting down... \n");
		TTF_Quit();
		IMG_Quit();
		SDL_GL_DeleteContext(glcontext);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
}