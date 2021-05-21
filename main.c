#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>

// Screen size
const int screenWidth = 600; // Must be higher than height
const int screenHeight = 600;

// Game size
#define gameWidth 40
#define gameHeight 40

// Screen scale
const unsigned char SCREEN_SCALE = (screenHeight/gameHeight);
const unsigned int SCREEN_CENTER_X = (screenWidth-(gameWidth*SCREEN_SCALE))/2;
const unsigned int SCREEN_CENTER_Y = (screenHeight-(gameHeight*SCREEN_SCALE))/2;

// Structs

	//* Particle *//
	typedef struct Particle
	{
		Vector2 pos;
		unsigned char type;
	}Particle;

	#define MAX_PARTICLES gameHeight*gameWidth
	static Particle particle[MAX_PARTICLES];
	#define COLOR_DUST (Color){255, 210, 105, 255}
	#define COLOR_STONE (Color){150, 150, 150, 255}
	unsigned char TYPE_SELECTED = 1;

// Particle functions
Particle * CheckParticle(int x, int y) // Return the pointer of a particle on certain position
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if ((particle[i].type != 0) && (particle[i].pos.x == x) && (particle[i].pos.y == y))
		{
			return (&particle[i]);
		}
	}
	return NULL;
}

// General Functions
void InitGame()
{
	// Particles set
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particle[i].pos.x = 0;
		particle[i].pos.y = 0;
		particle[i].type = 0;
	}
}

void UpdateParticles()
{
	// Update particles / ceils
	for (int i = gameHeight; i >= 0; i--)
	{
		for (int j = 0; j < gameWidth; j++)
		{
			Particle* ptr_particle = CheckParticle(j, i);
			if (ptr_particle != NULL) // If particle exist
			{
				switch (ptr_particle->type)
				{
					case 1:
						// If Y limit not reached
						if (ptr_particle->pos.y < gameHeight-1)
						{
							// Down empty
							if (CheckParticle(ptr_particle->pos.x, ptr_particle->pos.y+1) == NULL)
							{
								ptr_particle->pos.y += 1;
							}

							// Down left empty
							else if ((CheckParticle(ptr_particle->pos.x-1, ptr_particle->pos.y+1) == NULL) && (ptr_particle->pos.x-1 >= 0))
							{
								ptr_particle->pos.y += 1;
								ptr_particle->pos.x -= 1;
							}

							// Down right empty
							else if ((CheckParticle(ptr_particle->pos.x+1, ptr_particle->pos.y+1) == NULL) && (ptr_particle->pos.x+1 < gameWidth))
							{
								ptr_particle->pos.y += 1;
								ptr_particle->pos.x += 1;
							}
						}
						break;

					case 2:
						// DO NOTHING
						break;
				}
			}
		}
	}
}

void AddParticle()
{
	// Mouse button
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		// Get relative game screen mouse position
		int mouse_x = (GetMouseX()/SCREEN_SCALE)-(SCREEN_CENTER_X/SCREEN_SCALE);
		int mouse_y = GetMouseY()/SCREEN_SCALE;

		// Find a deactivated particle
		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			if ((particle[i].type == 0) && (CheckParticle(mouse_x, mouse_y) == NULL))
			{
				particle[i].type = TYPE_SELECTED;
				particle[i].pos.x = mouse_x;
				particle[i].pos.y = mouse_y;
				break;
			}
		}
	}
}

void EraseParticle()
{
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		// Get relative game screen mouse position
		int mouse_x = (GetMouseX()/SCREEN_SCALE)-(SCREEN_CENTER_X/SCREEN_SCALE);
		int mouse_y = GetMouseY()/SCREEN_SCALE;

		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			if ((particle[i].pos.x == mouse_x) && (particle[i].pos.y == mouse_y))
			{
				particle[i].pos.x = 0;
				particle[i].pos.y = 0;
				particle[i].type = 0;
				break;
			}
		}
	}
}

void ChangeParticleType()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		if (TYPE_SELECTED == 2)
		{
			TYPE_SELECTED = 1;
		}
		else if (TYPE_SELECTED == 1)
		{
			TYPE_SELECTED = 2;
		}
	}

	//printf("TYPE: %i\n", TYPE_SELECTED);
}

void UpdateGame()
{
	UpdateParticles();
	ChangeParticleType();
	AddParticle();
	EraseParticle();
}

void DrawParticles()
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		switch (particle[i].type)
		{
			case 1:
				DrawPixelV(particle[i].pos, COLOR_DUST);
				break;

			case 2:
				DrawPixelV(particle[i].pos, COLOR_STONE);
				break;
		}
	}
}

// Main function
int main()
{
	// Raylib INI
	InitWindow(screenWidth, screenHeight, "Sand Game");
	SetTargetFPS(60);
	RenderTexture2D render = LoadRenderTexture(gameWidth, gameHeight);

	// Ini Game
	InitGame();

	while (!WindowShouldClose())
	{
		// TODO
		UpdateGame();

		// DRAW
		BeginDrawing();
		ClearBackground(WHITE);
		BeginTextureMode(render);
			ClearBackground(BLACK);
			DrawParticles();
		EndTextureMode();

		// default to pro --> DrawTexturePro(render.texture, (Rectangle){0, 0, render.texture.width, render.texture.height}, (Rectangle){0, 0, render.texture.width, render.texture.height}, (Vector2){0, 0}, 0.0f, WHITE);
		DrawTexturePro(render.texture, (Rectangle){0, 0, render.texture.width, -render.texture.height}, (Rectangle){SCREEN_CENTER_X, SCREEN_CENTER_Y, render.texture.width*SCREEN_SCALE, render.texture.height*SCREEN_SCALE}, (Vector2){0, 0}, 0.0f, WHITE);
		DrawFPS(0,0);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}