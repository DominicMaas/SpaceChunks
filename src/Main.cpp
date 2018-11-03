#include "Engine.hpp"

Engine *engine;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void Init()
{
	glViewport(0, 0, 800, 600);
}

void Input(sf::Event event)
{ }
 
void Render()
{
	// Clears the screen
	//engine->ClearScreen(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void UpdatePhysics()
{ }

int main()
{
	// Create the engine, passing in the required functions
	engine = new Engine(Init, Render, Input, UpdatePhysics);

	// Create the window
	engine->CreateWindow(800, 600, "SpaceChunks", 60.0f);

	delete engine;
	return 0;
}