#define GLM_FORCE_RADIANS

#include "XyEngine.hpp"
#include "Player.hpp"
#include "Chunk.hpp"
#include "WorldManager.hpp"

Player *player;
WorldManager *world;
XyEngine *engine;

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
	world = new WorldManager(engine);

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
void Input(SDL_Event event)
{
	// Switch on the event type
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
			case SDLK_F2:	
				// Sets the player position
				player->SetPosition(glm::vec3(10, 128, 10));		
				break;
			case SDLK_F3:
				// Toggles debug text on screen
				isDebug = !isDebug;
				break;
			case SDLK_q:
				// Sets a block at the players position
				world->SetBlock((int)player->GetPosition().x, (int)player->GetPosition().y, (int)player->GetPosition().z, BlockType::Grass);
				break;
			case SDLK_e:
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
		//--------------------//
		
		// If the debug varaible is true
		if (isDebug)
		{
			// Set Rendering mode to 2D
			// RenderText(x,y, string);
			engine->Set2D();
				engine->RenderText(5, 0, "Space Chunks Alpha 0.3.2, XyEngine Version: " + std::string(engine->GetXyEngineVersion()));
				engine->RenderText(5, 15, "Copyright � 2014 - 2017 Dominic Maas, Provided under the MIT license");

				engine->RenderText(5, 45, "Render FPS: " + engine->ConvertIntToString((int)engine->GetFPS()));
				engine->RenderText(5, 60, "Physics FPS: " + engine->ConvertIntToString((int)engine->GetPhysicsFPS()));

				engine->RenderText(5, 90, "View Distance: " + engine->ConvertIntToString(world->GetViewDistance()));
				engine->RenderText(5, 105, "Chunks: " + engine->ConvertIntToString(world->GetLoadedChunks()) + "  (" + engine->ConvertIntToString(world->GetFrustumChunks()) + ")");

				std::string playerPosStr = " X: " + engine->ConvertIntToString((int)player->GetPosition().x) + " Y: " + engine->ConvertIntToString((int)player->GetPosition().y) + " Z: " + engine->ConvertIntToString((int)player->GetPosition().z);
				engine->RenderText(5, 135, "Player Position: " + playerPosStr);

				std::string playerRotStr = " X: " + engine->ConvertIntToString((int)player->GetRotation().x) + " Y: " + engine->ConvertIntToString((int)player->GetRotation().y) + " Z: " + engine->ConvertIntToString((int)player->GetRotation().z);
				engine->RenderText(5, 150, "Player Rotation: " + playerRotStr);

				engine->RenderText(5, 180, "Seed: " + engine->ConvertIntToString(engine->GetSeed()));
		}
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
	engine = new XyEngine(Init, Render, Input, UpdatePhysics);

	// Create the window
	engine->CreateWindow(1280, 720, "SpaceChunks", 60.0f);

	delete player;
	delete world;
	delete engine;
	return 0;
}