/*==============================================================================



Score [Bg.cpp]

Author : Zishan

Date   : 2025/07/016

--------------------------------------------------------------------------------



==============================================================================*/
#include "Bg.h"
#include "texture.h"
#include "sprite.h"

static int g_Bg01TexId = -1;
static int g_Bg02TexId = -1;
static int g_Bg03TexId = -1;
static int g_Bg04TexId = -1;

static double g_AccumulatedTime = 0.0;
static constexpr float g_ScrollSpeed01 = 200.0f;
static constexpr float g_ScrollSpeed02 = 300.0f;
static constexpr float g_ScrollSpeed03 = 400.0f;
static constexpr float g_ScrollSpeed04 = 500.0f;

static float g_Offset01X = 0.0f;
static float g_Offset02X = 0.0f;
static float g_Offset03X = 0.0f;
static float g_Offset04X = 0.0f;

void Bg_Initialize()
{
	g_Bg01TexId = Texture_Load(L"layer1.png");
	g_Bg02TexId = Texture_Load(L"layer2.png");
	g_Bg03TexId = Texture_Load(L"layer3.png");
	g_Bg04TexId = Texture_Load(L"layer4.png");
}

void Bg_Finalize()
{
}

void Bg_Update(double elapsed_time)
{
	g_AccumulatedTime += elapsed_time;

	g_Offset01X = g_AccumulatedTime * g_ScrollSpeed01;
	g_Offset02X = g_AccumulatedTime * g_ScrollSpeed02;
	g_Offset03X = g_AccumulatedTime * g_ScrollSpeed03;
	g_Offset04X = g_AccumulatedTime * g_ScrollSpeed04;
}

void Bg_Draw()
{
	Sprite_Draw(g_Bg01TexId, 0.0f, 0.0f, g_Offset01X, 0.0f, 1600, 900);
	Sprite_Draw(g_Bg02TexId, 0.0f, 0.0f, g_Offset02X, 0.0f, 1600, 900);
	Sprite_Draw(g_Bg03TexId, 0.0f, 0.0f, g_Offset03X, 0.0f, 1600, 900);
	Sprite_Draw(g_Bg04TexId, 0.0f, 0.0f, g_Offset04X, 0.0f, 1600, 900);
}
