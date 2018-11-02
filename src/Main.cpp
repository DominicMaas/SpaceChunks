#define GLM_FORCE_RADIANS

#include "Engine.hpp"
#include "Player.hpp"
#include "Chunk.hpp"
#include "World.hpp"

Player *player;
World *world;
Engine *engine;

// Boolean for displaying the debug info on the screen
bool isDebug = true;

/*
	This function creates everything and makes sure we are ready for rendering, 
	it is passed into the CreateWindowRender method
*/
void Init()
{
	// Clear the screen with a light blue color (may be replaced with skybox in the future)
	engine->ClearScreen(0.12f, 0.56f, 1.0f, 1.0f);

	// Create the player, setting its position, rotation and passing in the engine
	player = new Player(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), engine);

	// Create the world, passing in the engine
	world = new World(engine);

	// Set the world to 3D rendering
	engine->Set3D();

	// Create a Seed for the world (May not use anymore)
	engine->GenSeed();

	//----- Display how long the world took to load in the console -----//
	double StartTime = Time::GetTime();
	world->UpdateSetupList();
	double EndTime = Time::GetTime() - StartTime;
	std::cout << "World Took " << EndTime << " Seconds to Load!" << std::endl;
	//------------------------------------------------------------------//
}

/*
	This function handles the games input, it is called within the engine, and 
	requires a SDL_Event param
*/
void Input(sf::Event event)
{
	// Switch on the event type
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
			case sf::Keyboard::F2: 	
				// Sets the player position
				player->SetPosition(glm::vec3(10, 128, 10));		
				break;
			case sf::Keyboard::F3:
				// Toggles debug text on screen
				isDebug = !isDebug;
				break;
			case sf::Keyboard::Q:
				// Sets a block at the players position
				world->SetBlock((int)player->GetPosition().x, (int)player->GetPosition().y, (int)player->GetPosition().z, BlockType::Grass);
				break;
			case sf::Keyboard::E:
				// Removes a block below the players position
				world->SetBlock((int)player->GetPosition().x, (int)player->GetPosition().y - 1, (int)player->GetPosition().z, BlockType::Air);
				break;
		}
		break;
	}
}
 
/*
	Called 60 frames per second (or what you have set)
*/
void Render()
{
	// Clears the screen
	engine->ClearScreen(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Set rendering mode to 3D
	engine->Set3D();
		
	// Updates the player position
	player->Update(engine->IsMouseIn());

	// Updates the world
	world->Update(player->GetPosition(), player->GetRotation());
		
	//-- Lighting Stuff --//
	GLfloat ambient_color[] = { 0.7f, 0.7f, 0.7f, 0.7f };
	GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
}

/*
	Physics is run in a seperate thread to the game, heavy precessing here
	I will evenutally move all movement into here
*/
void UpdatePhysics()
{
	// Update the world physics
	world->UpdatePhysics(player);
}

/*
	Main Function for program
*/
int main()
{
	// Create the engine, passing in the required functions
	engine = new Engine(Init, Render, Input, UpdatePhysics);

	// Create the window
	engine->CreateWindow(1280, 720, "SpaceChunks", 60.0f);

	delete player;
	delete world;
	delete engine;
	return 0;
}