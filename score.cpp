/*==============================================================================



Score [score.cpp]

Author : Zishan

Date   : 2025/07/09

--------------------------------------------------------------------------------



==============================================================================*/
#include "score.h"
#include "sprite.h"
#include "texture.h"
#include <algorithm>

static unsigned int g_Score = 0;
static unsigned int g_ViewScore = 0;
static int g_Digit = 1;
static unsigned int g_CouterStop = 1;
static float g_x = 0.0f;
static float g_y = 0.0f;
static int g_ScoreTextureId = -1;
static constexpr int SCORE_FONT_WIDTH = 32;
static constexpr int SCORE_FONT_HEIGHT = 30;
static void drawNumber(float x, float y, int num);

void Score_Initialize(float x, float y, int digit)

{
	g_Score = 0;
	g_ViewScore = 0;
	g_Digit = digit;
	g_x = x;
	g_y = y;
	g_CouterStop = 1;

	for (int i = 0; i < digit; i++) {

		g_CouterStop *= 10;

	}

	g_CouterStop--;
	g_ScoreTextureId = Texture_Load(L"Number.png");
}

void Score_Finalize()
{

}

void Score_Update()
{
	if (g_ViewScore < g_Score) {
		g_ViewScore = std::min(g_ViewScore + 1, g_Score);
	}
}
void Score_Draw()
{

	unsigned int temporary = std::min(g_ViewScore, g_CouterStop);

	for (int i = 0; i < g_Digit; i++) {

		int n = temporary % 10;

		float x = g_x + (g_Digit - 1 - i) * SCORE_FONT_WIDTH;

		drawNumber(x, g_y, n);

		temporary /= 10;

	}
}

unsigned int Score_GetScore()
{

	return g_Score;
}

void Score_AddScore(int score)
{
	g_Score += score;
}
void Score_Reset()
{
	g_Score = 0;
}

void drawNumber(float x, float y, int num)
{
	// Map the number (e.g., 5) to the correct frame in the sprite sheet
	int texture_map_index = num;

	// Define the SOURCE rectangle (the part of the Number.png file to use)
	int srcX = texture_map_index * SCORE_FONT_WIDTH;
	int srcY = 0;

	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Sprite_Draw(
		g_ScoreTextureId,      // texid
		x, y,                  // dx, dy (screen position)
		64.0f,64.0f,
		srcX, srcY,            // pix, piy (source position in file)
		SCORE_FONT_WIDTH,   // piw (source width)
		SCORE_FONT_HEIGHT,  // pih (source height)
		color                  // color
	);
}