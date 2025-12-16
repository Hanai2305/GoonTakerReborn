#pragma once
#include <utils.h>
using namespace utils;

#pragma region gameInformation
// TODO: Set your name and group in the title here
std::string g_WindowTitle{ "Project name - Name, firstname - 1DAExx" };

// Change the window dimensions here
float g_WindowWidth{ 700 };
float g_WindowHeight{ 700 };
#pragma endregion gameInformation



#pragma region ownDeclarations
// Declare your own global variables heree
enum class AnimState
{
    Idle,
    Run,
    Attack,
    Dead
};

struct Sprite
{
    Texture texture;
    int frames;
    int cols;
    float frameTime;
    int currentFrame;
    float accumulatedTime;
};
struct Skeleton
{
    Point2f pos{};
    Sprite animation{};
    AnimState state{ AnimState::Idle };
};

AnimState g_AnimStateMainCharacter = AnimState::Idle;


Sprite g_MainCharacterSprite{};
Sprite g_WallSprite{};
Sprite g_SpikesSprite{};
Sprite g_VaseSprite{};
Sprite g_SkeletonSpriteIdle{};
Sprite g_SkeletonSpriteDead{};
Sprite g_WallTextureOnGrid{};

Point2f g_MainCharacterPos{5, 5};
Point2f g_WallPos{ 0, 0 };
Point2f g_SpikesPos{ 0, 0 };
Point2f g_VasePos{ 0, 0 };
Point2f g_WallTextureOnGridPos{ 0, 0 };
Point2f* g_SkeletonPositions = nullptr;
Point2f* g_VasePositions = nullptr;
Point2f* g_WallsPositions = nullptr;
Skeleton* g_Skeletons = nullptr;

int* g_VaseFrames = nullptr;

int g_VaseAmount{ 2 };
int g_SkeletonsAmount{ 3 };
int g_WallsOnGridAmount{ 5 };

const float g_LineWidth{ g_WindowWidth / 10.f };
const float g_LineHeight{ g_WindowHeight / 10.f };
const float g_cellSize{ g_WindowWidth / 10.f };
float g_CoolDown{ 0.2f };

bool g_LookingAtRight{ true };
bool g_LookingAtUp{ true };
bool g_CheckXas{ true };

// Declare your own functions here
void DrawGrid();
void DrawSprite(const Sprite& sprite, Point2f& pos, float scale, bool flipping);
void InitGameResources();
void FreeGameResources();
void UpdateSprite(Sprite& sprite, float elapsedSec);

void DrawFloor();
void DrawSpikes();
void FlipSprite();
void DrawVase(Point2f pos);
void DrawSkeleton(const Skeleton& skeleton);
void DrawWall(Point2f pos);

void VaseCol(float elapsedSec);
void SkeletonCol(float elapsedSec);
void WallOnGridCol();

void AnimatedMainCharacterAttack(float elapsedSec);
void AnimateMainCharacterIdle(float elapsedSec);
void AnimatedMainCharacterRun(const float elapsedSec);
Point2f ConvertToCenter(const Sprite& sprite, Point2f pos, float scale);

void CoolDown(float elapsedSec);

#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
