#include "Engine.hpp"

Engine::Engine(void(*initFunc)(), void(*renderFunc)(), void(*inputFunc)(sf::Event event), void(*physicsFunc)()) : _physicsThread(&Engine::PhysicsThread, this)
{
	this->m_InitFunc = initFunc;
	this->m_RenderFunc = renderFunc;
	this->m_InputFunc = inputFunc;
	this->m_PhysicsFunc = physicsFunc;
}

void Engine::PhysicsThread()
{
	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	pysicsFrames = 0;

	while (_isRunning)
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
		else
		{
			sf::sleep(sf::milliseconds(1));
		}
	}
}

int Engine::CreateWindow(int width, int height, char* title, float frameRate)
{
	double StartTime = Time::GetTime();
	Log("Engine is Loading...");
	
	this->m_windowWidth = width;
	this->m_windowHeight = height;

	this->m_middleWidth = m_windowWidth / 2;
	this->m_middleHeight = m_windowHeight / 2;

	this->m_FrameRate = frameRate;
	this->m_frameTime = 1.0 / this->m_FrameRate;

	// Create the window
	_window.create(sf::VideoMode(m_windowWidth, m_windowHeight), title, sf::Style::Default, sf::ContextSettings(32));

	// Load glew
	if (glewInit() != GLEW_OK)
		return ReturnWithError("GLEW Failed to Init!");

	_isRunning = true;

	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	frames = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE_ARB);

	m_InitFunc();

	_physicsThread.launch();

	double EndTime = Time::GetTime() - StartTime;
	std::cout << "Time Taken to load XyEngine: " << EndTime << std::endl;

	sf::Event event;

	while (_isRunning)
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

			while (_window.pollEvent(event))
			{
				m_InputFunc(event);

				switch (event.type)
				{
					case sf::Event::Closed:
						_isRunning = false;
						break;

					case sf::Event::MouseButtonPressed:
						mousein = true;
						_window.setMouseCursorVisible(false);
						_window.setMouseCursorGrabbed(false);
						break;

					case sf::Event::KeyPressed:
						switch (event.key.code)
						{		
							case sf::Keyboard::Escape:
							mousein = false;
							_window.setMouseCursorVisible(true);
							_window.setMouseCursorGrabbed(true);
							break;
						}
				}
			}

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			m_RenderFunc();
			
			_window.display();
			
			frames++;
		}
		else
		{
			sf::sleep(sf::milliseconds(1));
		}
	}


	return EXIT_SUCCESS;
}

int Engine::ReturnWithError(std::string err)
{
	Log(err.c_str());
	return EXIT_FAILURE;
}

void Engine::Log(const char* text)
{
	std::cout << "[XYEngine] " << text << std::endl;
}

void Engine::RenderText(float x, float y, const std::string message)
{ }

Engine::~Engine()
{
	printf("[CORE] Engine is shutting down... \n");
	_window.close();
}