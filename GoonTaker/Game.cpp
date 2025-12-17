#include "pch.h"
#include "Game.h"
#include <iostream>

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	// initialize game resources heree
	InitGameResources();
	g_MainCharacterSprite.texture.height -= 60;
}

void Draw()
{
	ClearBackground(0.f, 0.f, 0.f);

	// Put your own draw statements here
	
	DrawFloor();
	Point2f mainCharacterPos = ConvertToCenter(g_MainCharacterSprite, g_MainCharacterPos, 1.75f);
	
	DrawSprite(g_MainCharacterSprite, mainCharacterPos, 1.75f, g_LookingAtRight);
	for (int i{}; i < g_VaseAmount; ++i)
	{
		g_VaseSprite.currentFrame = g_VaseFrames[i];
		DrawVase(g_VasePositions[i]);
	}
	for (int i{}; i < g_SpikesAmount; ++i)
	{
		DrawSpikes(g_SpikesPositions[i]);
	}
	for (int i{}; i < g_SkeletonsAmount; ++i)
	{
		DrawSkeleton(g_Skeletons[i]);
	}
	DrawGrid();
	
}

void Update(float elapsedSec)
{
	// process input, do physics
	
	
	switch (g_AnimStateMainCharacter)
	{
	case AnimState::Idle:
		AnimateMainCharacterIdle(elapsedSec);
		break;
	case AnimState::Attack:
		AnimatedMainCharacterAttack(elapsedSec);
		break;
	case AnimState::Run:
		AnimatedMainCharacterRun(elapsedSec);
		break;
	case AnimState::Dead:
		AnimatedMainCharacterDead(elapsedSec);
		break;
	}
	VaseCol(elapsedSec);
	SpikeCol(elapsedSec);
	SkeletonCol(elapsedSec);
	CoolDown(elapsedSec);

	for (int i{}; i < g_SkeletonsAmount; ++i)
	{
		if (g_Skeletons[i].state == AnimState::Dead)
		{
			if (g_Skeletons[i].animation.currentFrame < g_Skeletons[i].animation.frames - 1)
			{
				UpdateSprite(g_Skeletons[i].animation, elapsedSec);
			}
			else
			{
				g_Skeletons[i].pos = Point2f{-1, -1};
			}
		}
		else
		{
			g_Skeletons[i].state = AnimState::Idle;
			UpdateSprite(g_Skeletons[i].animation, elapsedSec);
		}
		AnimatedSpikesOnTheFloor(elapsedSec);
	}
	
	//UpdateSprite(g_VaseSprite, elapsedSec);

	// e.g. Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void End()
{
	// free game resources here
	FreeGameResources();
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
	if (g_AnimStateMainCharacter == AnimState::Dead || g_AnimStateMainCharacter == AnimState::Attack)
	{
		return;
	}	
	g_MainCharacterSprite.currentFrame = 23;
	
	if (g_CoolDown > 0.f)
	{
		return;
	}
	switch (key)
	{
	case SDLK_UP:
		g_CheckXas = false;
		g_LookingAtUp = true;
		g_MainCharacterPos.y--;
		g_AnimStateMainCharacter = AnimState::Run;
		g_CoolDown = 0.2f;
		break;
	case SDLK_DOWN:
		g_CheckXas = false;
		g_LookingAtUp = false;
		g_MainCharacterPos.y++;
		g_AnimStateMainCharacter = AnimState::Run;
		g_CoolDown = 0.2f;
		break;
	case SDLK_RIGHT:
		g_CheckXas = true;
		g_LookingAtRight = true;
		g_MainCharacterPos.x++;
		g_AnimStateMainCharacter = AnimState::Run;
		g_CoolDown = 0.2f;
		break;
	case SDLK_LEFT:
		g_CheckXas = true;
		g_LookingAtRight = false;
		g_MainCharacterPos.x--;
		g_AnimStateMainCharacter = AnimState::Run;
		g_CoolDown = 0.2f;
		break;
	}
	if (g_MainCharacterPos.x >= 9)
	{
		g_MainCharacterPos.x = 8;
	}
	if (g_MainCharacterPos.x <= 1)
	{
		g_MainCharacterPos.x = 1;
	}
	if (g_MainCharacterPos.y >= 9)
	{
		g_MainCharacterPos.y = 8;
	}
	if (g_MainCharacterPos.y <= 1)
	{
		g_MainCharacterPos.y = 1;
	}

}

void OnKeyUpEvent(SDL_Keycode key)
{
	//switch (key)
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	// SAMPLE CODE: print mouse position
	//const float mouseX{ float(e.x) };
	//const float mouseY{ float(e.y) };
	//std::cout << "  [" << mouseX << ", " << mouseY << "]\n";
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	// SAMPLE CODE: print mouse position
	//const float mouseX{ float(e.x) };
	//const float mouseY{ float(e.y) };
	//std::cout << "  [" << mouseX << ", " << mouseY << "]\n";
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	// SAMPLE CODE: print mouse position
	//const float mouseX{ float(e.x) };
	//const float mouseY{ float(e.y) };
	//std::cout << "  [" << mouseX << ", " << mouseY << "]\n";

	// SAMPLE CODE: check which mouse button was pressed
	//switch (e.button)
	//{
	//case SDL_BUTTON_LEFT:
	//	//std::cout << "Left mouse button released\n";
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	//std::cout << "Right mouse button released\n";
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	//std::cout << "Middle mouse button released\n";
	//	break;
	//}
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here

void InitGameResources()
{
	g_SkeletonPositions = new Point2f[g_SkeletonsAmount];
	g_VasePositions = new Point2f[g_VaseAmount];
	g_VaseFrames = new int[g_VaseAmount];
	g_Skeletons = new Skeleton[g_SkeletonsAmount];
	g_SpikesPositions = new Point2f[g_SpikesAmount];

	Point2f positionsOfSkeletons[] = { Point2f{8,6}, Point2f{2, 3}, Point2f{3, 7} };

	for (int i{}; i < g_VaseAmount; ++i)
	{
		g_VaseFrames[i] = 0;
	}

	g_VasePositions[0] = Point2f{ 4, 7 };
	g_VasePositions[1] = Point2f{ 7, 5 };

	g_SpikesPositions[0] = Point2f{ 5,8 };
	g_SpikesPositions[1] = Point2f{ 3,2 };
	g_SpikesPositions[2] = Point2f{ 8,1 };
	g_SpikesPositions[3] = Point2f{ 2,5 };
	g_SpikesPositions[4] = Point2f{ 1,3 };

	if (!TextureFromFile("Resources/NightBorne/NightBorne.png", g_MainCharacterSprite.texture))
	{
		std::cout << "ERROR loading main character spritesheet!\n";
	}
	g_MainCharacterSprite.frames = 23*5;
	g_MainCharacterSprite.cols = 23;
	g_MainCharacterSprite.frameTime = 0.075f;
	g_MainCharacterSprite.accumulatedTime = 0.f;
	g_MainCharacterSprite.currentFrame = 0;

	if (!TextureFromFile("Resources/individuals files/dongeonWallFloorTransparent1.png", g_WallSprite.texture))
	{
		std::cout << "ERROR loading dungeonwall spritesheet!\n";
	}
	g_WallSprite.frames = 1;
	g_WallSprite.cols = 1;
	g_WallSprite.frameTime = 0.1f;
	g_WallSprite.accumulatedTime = 0.f;
	g_WallSprite.currentFrame = 0;

	if (!TextureFromFile("Resources/Skeleton Idle.png", g_SkeletonSpriteIdle.texture))
	{
		std::cout << "ERROR loading skeleton spritesheet!\n";
	}
	g_SkeletonSpriteIdle.frames = 11;
	g_SkeletonSpriteIdle.cols = 11;
	g_SkeletonSpriteIdle.frameTime = 0.1f;
	g_SkeletonSpriteIdle.accumulatedTime = 0.f;
	g_SkeletonSpriteIdle.currentFrame = 0;
	
	if (!TextureFromFile("Resources/Vases_AssetPack_By_VerzatileDev/VasesAssetPackTransparent.png", g_VaseSprite.texture))
	{
		std::cout << "ERROR loading vases spritesheet!\n";
	}
	g_VaseSprite.frames = 80;
	g_VaseSprite.cols = 8;
	g_VaseSprite.frameTime = 0.1f;
	g_VaseSprite.accumulatedTime = 0.f;
	g_VaseSprite.currentFrame = 0;
	if (!TextureFromFile("Resources/Skeleton Dead.png", g_SkeletonSpriteDead.texture))
	{
		std::cout << "ERROR loading skeleton death spritesheet!\n";
	}
	g_SkeletonSpriteDead.frames = 15;
	g_SkeletonSpriteDead.cols = 15;
	g_SkeletonSpriteDead.frameTime = 0.05f;
	g_SkeletonSpriteDead.accumulatedTime = 0.f;
	g_SkeletonSpriteIdle.currentFrame = 0;

	if (!TextureFromFile("Resources/Dungeon tileset.png", g_WallTextureOnGrid.texture))
	{
		std::cout << "ERROR loading dungeon tileset spritesheet\n";
	}
	g_WallTextureOnGrid.frames = 288;
	g_WallTextureOnGrid.cols = 24;
	g_WallTextureOnGrid.frameTime = 0.1f;
	g_WallTextureOnGrid.accumulatedTime = 0.f;
	g_WallTextureOnGrid.currentFrame = 63;
	if (!TextureFromFile("Resources/Dungeon tileset.png", g_DungeonTileSet.texture))
	{
		std::cout << "ERROR loading dungeon tileset spritesheet\n";
	}
	g_DungeonTileSet.frames = 288;
	g_DungeonTileSet.cols = 24;
	g_DungeonTileSet.frameTime = 0.1f;
	g_DungeonTileSet.accumulatedTime = 0.f;
	g_DungeonTileSet.currentFrame = 63;

	for (int i{}; i < g_SkeletonsAmount; ++i)
	{
		g_Skeletons[i].pos = positionsOfSkeletons[i];
		g_Skeletons[i].animation = g_SkeletonSpriteIdle;
		g_Skeletons[i].state = AnimState::Idle;
	}
}
void FreeGameResources()
{
	DeleteTexture(g_MainCharacterSprite.texture);
	DeleteTexture(g_WallSprite.texture);
	DeleteTexture(g_VaseSprite.texture);
	DeleteTexture(g_SkeletonSpriteIdle.texture);
	DeleteTexture(g_SkeletonSpriteDead.texture);
	DeleteTexture(g_DungeonTileSet.texture);
	delete[] g_VasePositions;
	delete[] g_VaseFrames;
	delete[] g_SkeletonPositions;
}
void DrawSprite(const Sprite& sprite, Point2f& pos, float scale, bool flipping)
{
	int rows = sprite.frames / sprite.cols;

	float widthSprite = sprite.texture.width / sprite.cols;
	float heightSprite = sprite.texture.height / rows;

	int frame = sprite.currentFrame;
	int col = frame % sprite.cols;
	int row = frame / sprite.cols;

	Rectf src{};

	src.left = col * widthSprite;
	src.top = row * heightSprite;
	src.width = widthSprite;
	src.height = heightSprite;

	Rectf dst{};

	dst.left = pos.x;
	dst.top = pos.y;
	dst.width = widthSprite * scale;
	dst.height = heightSprite * scale;

	DrawTexture(sprite.texture, dst, src);
}
void DrawGrid()
{
	SetColor(1.f, 1.f, 1.f, 0.25f);
	float horLine = g_WindowWidth / 10.f;
	float vertLine = g_WindowHeight / 10.f;
	for (int r{}; r < 10; ++r)
	{
		DrawLine(vertLine, 0, vertLine, g_WindowHeight);
		vertLine += g_WindowWidth / 10.f;
	}
	for (int c{}; c < 10; ++c)
	{
		DrawLine(0, horLine, g_WindowWidth, horLine);
		horLine += g_WindowHeight / 10.f;
	}
}
void UpdateSprite(Sprite& sprite, float elapsedSec)
{
	sprite.accumulatedTime += elapsedSec;
	if (sprite.accumulatedTime > sprite.frameTime)
	{
		++sprite.currentFrame %= sprite.frames;
		sprite.accumulatedTime -= sprite.frameTime;
	}
}
Point2f ConvertToCenter(const Sprite& sprite, Point2f pos, float scale)
{
	float cellWidth = g_WindowWidth / 10.f;
	float cellHeight = g_WindowHeight / 10.f;

	int rows = sprite.frames / sprite.cols;

	float widthSprite = (sprite.texture.width / sprite.cols) * scale;
	float heightSprite = (sprite.texture.height / rows) * scale;

	Point2f correctedPos{};

	correctedPos.x = (pos.x * cellWidth) + cellWidth / 2 - widthSprite / 2;
	correctedPos.y = (pos.y * cellHeight) + cellHeight / 2 - heightSprite / 2;

	return correctedPos;
}
void DrawFloor()
{
	float tileWidth = g_WallSprite.texture.width;
	float tileHeight = g_WallSprite.texture.height;
	float scale = g_cellSize / tileWidth;

	
	Point2f TorchPos = ConvertToCenter(g_WallSprite, g_WallPos, scale);
	float wallX = g_WallPos.x;
	float wallY = g_WallPos.y;
	
	for (int i{}; i < 10; ++i)
	{
		for (int a{}; a < 10; ++a)
		{
			Point2f TorchPos = ConvertToCenter(g_WallSprite, Point2f{ wallX, wallY }, scale);
			DrawSprite(g_WallSprite, TorchPos, scale, false);
			wallX++;
		}
		wallY++;
		wallX = 0;
	}
}
void DrawVase(Point2f pos)
{
	Point2f vasePos = ConvertToCenter(g_VaseSprite, Point2f{pos.x, pos.y}, 0.25f);
	DrawSprite(g_VaseSprite, vasePos, 0.25f, false);
}
void DrawSkeleton(const Skeleton& skeleton)
{
	Point2f centeredPos = ConvertToCenter(skeleton.animation, skeleton.pos, 2.f);
	DrawSprite(skeleton.animation, centeredPos, 2.f, false);
}
void DrawSpikes(Point2f pos)
{
	Point2f wallOnGridPos = ConvertToCenter(g_WallTextureOnGrid, Point2f{pos.x, pos.y}, 4.f);
	DrawSprite(g_WallTextureOnGrid, wallOnGridPos, 4.f, false);
}
void DrawSpikesOnFloor(Point2f pos)
{
	g_DungeonTileSet.currentFrame = 63;
	float scale = g_cellSize / 16.f; 
	Point2f dungeonTileSetPos = ConvertToCenter(g_DungeonTileSet, Point2f{ pos.x, pos.y }, scale);
	DrawSprite(g_DungeonTileSet, dungeonTileSetPos, scale, false);
}
void FlipSprite()
{
	if (g_LookingAtRight == true)
	{
		g_LookingAtRight = false;
	}
	else
	{
		g_LookingAtRight = true;
	}
}
void VaseCol(float elapsedSec)
{
	int brokenFrames{4};
	for (int i{}; i < g_VaseAmount; ++i)
	{
		if (g_CheckXas)
		{
			if (g_LookingAtRight)
			{
				if (g_MainCharacterPos.x == g_VasePositions[i].x && g_MainCharacterPos.y == g_VasePositions[i].y)
				{
					g_MainCharacterSprite.currentFrame = 47;
					g_MainCharacterPos.x--;
					g_AnimStateMainCharacter = AnimState::Attack;
					g_VaseFrames[i] += 2;
					
					if (g_VaseFrames[i] == brokenFrames)
					{
						g_VaseFrames[i] = 0;
						g_VasePositions[i] = Point2f{ -1, -1 };
					}
				}
			}
			else if (!g_LookingAtRight)
			{
				if (g_MainCharacterPos.x == g_VasePositions[i].x && g_MainCharacterPos.y == g_VasePositions[i].y)
				{
					g_MainCharacterSprite.currentFrame = 47;
					g_MainCharacterPos.x++;
					g_AnimStateMainCharacter = AnimState::Attack;
					g_VaseFrames[i] += 2;
					if (g_VaseFrames[i] == brokenFrames)
					{
						g_VaseFrames[i] = 0;
						g_VasePositions[i] = Point2f{ -1, -1 };
					}
				}
				
			}
		}
		else if (!g_CheckXas)
		{
			if (g_LookingAtUp)
			{
				if (g_MainCharacterPos.x == g_VasePositions[i].x && g_MainCharacterPos.y == g_VasePositions[i].y)
				{
					g_MainCharacterSprite.currentFrame = 47;
					g_MainCharacterPos.y++;
					g_AnimStateMainCharacter = AnimState::Attack;
					g_VaseFrames[i] += 2;
					if (g_VaseFrames[i] == brokenFrames)
					{
						g_VaseFrames[i] = 0;
						g_VasePositions[i] = Point2f{ -1, -1 };
					}
				}
			}
			else if (!g_LookingAtUp)
			{
				if (g_MainCharacterPos.x == g_VasePositions[i].x && g_MainCharacterPos.y == g_VasePositions[i].y)
				{
					g_MainCharacterSprite.currentFrame = 47;
					g_MainCharacterPos.y--;
					g_AnimStateMainCharacter = AnimState::Attack;
					g_VaseFrames[i] += 2;
					if (g_VaseFrames[i] == brokenFrames)
					{
						g_VaseFrames[i] = 0;
						g_VasePositions[i] = Point2f{ -1, -1 };
					}
				}
			}
			
		}
	}	
}
void SkeletonCol(float elapsedSec)
{
	for (int i{}; i < g_SkeletonsAmount; ++i)
	{
		if (g_CheckXas)
		{
			if (g_LookingAtRight)
			{
				if (g_MainCharacterPos.x == g_Skeletons[i].pos.x && g_MainCharacterPos.y == g_Skeletons[i].pos.y)
				{
					g_MainCharacterPos.x--;
					g_MainCharacterSprite.currentFrame = 47;
					g_AnimStateMainCharacter = AnimState::Attack;

					g_Skeletons[i].animation = g_SkeletonSpriteDead;
					g_Skeletons[i].state = AnimState::Dead;
					UpdateSprite(g_Skeletons[i].animation, elapsedSec);
				}
			}
			else if (!g_LookingAtRight)
			{
				if (g_MainCharacterPos.x == g_Skeletons[i].pos.x && g_MainCharacterPos.y == g_Skeletons[i].pos.y)
				{
					g_MainCharacterPos.x++;
					g_MainCharacterSprite.currentFrame = 47;
					g_AnimStateMainCharacter = AnimState::Attack;

					g_Skeletons[i].animation = g_SkeletonSpriteDead;
					g_Skeletons[i].state = AnimState::Dead;
					UpdateSprite(g_Skeletons[i].animation, elapsedSec);
				}
		}
		
		}
		else if (!g_CheckXas)
		{
			if (g_LookingAtUp)
			{
				if (g_MainCharacterPos.x == g_Skeletons[i].pos.x && g_MainCharacterPos.y == g_Skeletons[i].pos.y)
				{
					g_MainCharacterPos.y++;
					g_MainCharacterSprite.currentFrame = 47;
					g_AnimStateMainCharacter = AnimState::Attack;

					g_Skeletons[i].animation = g_SkeletonSpriteDead;
					g_Skeletons[i].state = AnimState::Dead;
					UpdateSprite(g_Skeletons[i].animation, elapsedSec);
				}
			}
			if (!g_LookingAtUp)
			{
				if (g_MainCharacterPos.x == g_Skeletons[i].pos.x && g_MainCharacterPos.y == g_Skeletons[i].pos.y)
				{
					g_MainCharacterPos.y--;
					g_MainCharacterSprite.currentFrame = 47;
					g_AnimStateMainCharacter = AnimState::Attack;

					g_Skeletons[i].animation = g_SkeletonSpriteDead;
					g_Skeletons[i].state = AnimState::Dead;
					UpdateSprite(g_Skeletons[i].animation, elapsedSec);
				}
			}
		}
		
		
	}
}
void SpikeCol(float elapsedSec)
{
	for (int i{}; i < g_SpikesAmount; ++i)
	{
		if (g_CheckXas)
		{
			if (g_LookingAtRight)
			{
				if (g_MainCharacterPos.x == g_SpikesPositions[i].x && g_MainCharacterPos.y == g_SpikesPositions[i].y)
				{
					AnimatedSpikesOnTheFloor(elapsedSec);
					if (g_AnimStateMainCharacter != AnimState::Dead)
					{
						g_AnimStateMainCharacter = AnimState::Dead;
						g_MainCharacterSprite.currentFrame = 92;
					}
				}
			}
			if (!g_LookingAtRight)
			{
				if (g_MainCharacterPos.x == g_SpikesPositions[i].x && g_MainCharacterPos.y == g_SpikesPositions[i].y)
				{
					AnimatedSpikesOnTheFloor(elapsedSec);
					if (g_AnimStateMainCharacter != AnimState::Dead)
					{
						g_AnimStateMainCharacter = AnimState::Dead;
						g_MainCharacterSprite.currentFrame = 92;
					}
				}
			}
		}
		else if (!g_CheckXas)
		{
			if (g_LookingAtUp)
			{
				if (g_MainCharacterPos.x == g_SpikesPositions[i].x && g_MainCharacterPos.y == g_SpikesPositions[i].y)
				{
					AnimatedSpikesOnTheFloor(elapsedSec);
					if (g_AnimStateMainCharacter != AnimState::Dead)
					{
						g_AnimStateMainCharacter = AnimState::Dead;
						g_MainCharacterSprite.currentFrame = 92;
					}
				}

			}
			if (!g_LookingAtUp)
			{
				if (g_MainCharacterPos.x == g_SpikesPositions[i].x && g_MainCharacterPos.y == g_SpikesPositions[i].y)
				{
					if (g_AnimStateMainCharacter != AnimState::Dead)
					{
						g_AnimStateMainCharacter = AnimState::Dead;
						g_MainCharacterSprite.currentFrame = 92;
					}
				}
			}
		}
	}
}
void AnimatedSpikesOnTheFloor(const float elapsedSec)
{
	g_SpikesSprite.accumulatedTime += elapsedSec;
	if (g_SpikesSprite.accumulatedTime > g_SpikesSprite.frameTime)
	{
		g_SpikesSprite.currentFrame--;	
	}
	g_SpikesSprite.accumulatedTime -= g_SpikesSprite.frameTime;
}
void AnimateMainCharacterIdle(const float elapsedSec)
{	
	const int generalAnim{ 23 };
	const int idleAnim{ 9 };
	g_MainCharacterSprite.accumulatedTime += elapsedSec;
	if (g_MainCharacterSprite.accumulatedTime > g_MainCharacterSprite.frameTime)
	{
		++g_MainCharacterSprite.currentFrame %= generalAnim;
		g_MainCharacterSprite.accumulatedTime -= g_MainCharacterSprite.frameTime;
		if (g_MainCharacterSprite.currentFrame >= idleAnim)
		{
			g_MainCharacterSprite.currentFrame = 0;
		}
	}
}
void AnimatedMainCharacterAttack(const float elapsedSec)
{
	const int generalAnim{ 23 * 2 };
	const int attackAnim{ 12 };
	g_MainCharacterSprite.accumulatedTime += elapsedSec;

	if (g_MainCharacterSprite.accumulatedTime > g_MainCharacterSprite.frameTime)
	{
		g_MainCharacterSprite.currentFrame++;
		
		if (g_MainCharacterSprite.currentFrame >= attackAnim + generalAnim)
		{
			g_MainCharacterSprite.currentFrame = generalAnim;
			
			g_AnimStateMainCharacter = AnimState::Idle;
		}
		g_MainCharacterSprite.accumulatedTime -= g_MainCharacterSprite.frameTime;
	}
}
void AnimatedMainCharacterRun(const float elapsedSec)
{
	const int generalAnim{ 23 };
	const int runAnim{ 6 };
	g_MainCharacterSprite.accumulatedTime += elapsedSec;

	if (g_MainCharacterSprite.accumulatedTime > g_MainCharacterSprite.frameTime)
	{
		g_MainCharacterSprite.currentFrame++;

		if (g_MainCharacterSprite.currentFrame >= runAnim + generalAnim)
		{
			g_MainCharacterSprite.currentFrame = generalAnim;
		
			g_AnimStateMainCharacter = AnimState::Idle;

		}
		g_MainCharacterSprite.accumulatedTime -= g_MainCharacterSprite.frameTime;
	}
}
void AnimatedMainCharacterDead(const float elapsedSec)
{
	const int generalAnim{ 92 };
	const int deadAnim{ 23 };

	g_MainCharacterSprite.accumulatedTime += elapsedSec;

	if (g_MainCharacterSprite.accumulatedTime > g_MainCharacterSprite.frameTime)
	{
		g_MainCharacterSprite.currentFrame++;
		if (g_MainCharacterSprite.currentFrame > deadAnim + generalAnim)
		{
			g_AnimStateMainCharacter = AnimState::Idle;
			if (g_AnimStateMainCharacter != AnimState::Dead)
			{
				std::cout << "Game over!";
				g_MainCharacterPos = Point2f{ 1, 1 };
				
			}
		}
		g_MainCharacterSprite.accumulatedTime -= g_MainCharacterSprite.frameTime;
	}

}
void CoolDown(const float elapsedSec)
{
	if (g_CoolDown > 0.f)
	{
		g_CoolDown -= elapsedSec;
	}
}
#pragma endregion ownDefinitions