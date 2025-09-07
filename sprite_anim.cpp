/*=======================================================================================


Sprite_anim ï\é¶ [sprite_anim.cpp]

																	Author : Zishan
																	Date	: 2025/6/17
-----------------------------------------------------------------------------------------
=========================================================================================*/
#include "sprite_anim.h"
#include "sprite.h"
#include "texture.h"
#include <DirectXMath.h>
using namespace DirectX;

struct AnimPatternData
{
	int m_PatternMax = 0; // Pattern Count
	int m_TextureId = -1; // Texture Id
	int m_HPatternMax = 0; // â°ÇÃ pattern ç≈ëÂíl
	XMUINT2 m_StartPosition { 0,0 }; // Animation Starting Point
	XMUINT2 m_PatternSize{ 0,0 }; // Pattern width, height
	double m_seconds_per_pattern = 0.1;
	bool m_IsLooped = true; // Loop
};

struct AnimPlayData
{
	int m_PatternId = -1; // Animation Id
	int m_PatternNum = 0; 
	double m_AccumulatedTime = 0.0;
	bool m_IsStopped = false;
};

static constexpr int ANIM_PATTERN_MAX = 128;
static AnimPatternData g_AnimPattern[ANIM_PATTERN_MAX];
static constexpr int ANIM_PLAY_MAX = 256;
static AnimPlayData g_AnimPlay[ANIM_PLAY_MAX];

void SpriteAnim_Initialize()
{
	for (AnimPatternData& data : g_AnimPattern) {
		data.m_TextureId = -1;
	}
	for (AnimPlayData& data : g_AnimPlay) {
		data.m_PatternId = -1;
		data.m_IsStopped = false;
	}
}

void SpriteAnim_Finalize()
{
}

void SpriteAnim_Update(double elapsed_time)
{
	for (int i = 0; i < ANIM_PLAY_MAX; i++) {

		if (g_AnimPlay[i].m_PatternId < 0) continue;

		AnimPatternData* pAnimPatternData = &g_AnimPattern[g_AnimPlay[i].m_PatternId];

		if (g_AnimPlay[i].m_AccumulatedTime >= pAnimPatternData -> m_seconds_per_pattern) {
			g_AnimPlay[i].m_PatternNum++;
			

			if (g_AnimPlay[i].m_PatternNum >= pAnimPatternData->m_PatternMax) {
				if (pAnimPatternData->m_IsLooped) {
					g_AnimPlay[i].m_PatternNum = 0;
				}
				else {
					g_AnimPlay[i].m_PatternNum = pAnimPatternData->m_PatternMax - 1;
					g_AnimPlay[i].m_IsStopped = true;
				}
			}
			g_AnimPlay[i].m_AccumulatedTime -= pAnimPatternData->m_seconds_per_pattern;
		}

		g_AnimPlay[i].m_AccumulatedTime += elapsed_time;
	}
}

void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh)
{
	if (playid < 0 || playid >= ANIM_PLAY_MAX) return;
	if (g_AnimPlay[playid].m_PatternId < 0) return;

	int anim_pattern_id = g_AnimPlay[playid].m_PatternId;
	AnimPatternData* p = &g_AnimPattern[anim_pattern_id];

	int frame = g_AnimPlay[playid].m_PatternNum;

	// Calculate frame position in spritesheet (x = column, y = row)
	int pattern_x = frame % p->m_HPatternMax;
	int pattern_y = frame / p->m_HPatternMax;

	int src_x = p->m_StartPosition.x + pattern_x * p->m_PatternSize.x;
	int src_y = p->m_StartPosition.y + pattern_y * p->m_PatternSize.y;

	Sprite_Draw(
		p->m_TextureId,
		dx, dy, dw, dh,
		src_x, src_y,
		p->m_PatternSize.x,
		p->m_PatternSize.y
	);
}

int SpriteAnim_RegisterPattern(int textureId, int patternMax, int h_patternMax,double seconds_per_pattern,
	const DirectX::XMUINT2& pattern_size, const DirectX::XMUINT2& start_position, bool is_looped)
{
	for (int i = 0; i < ANIM_PATTERN_MAX; i++) 
	{
		if (g_AnimPattern[i].m_TextureId >= 0) continue;

		g_AnimPattern[i].m_TextureId = textureId;
		g_AnimPattern[i].m_PatternMax = patternMax;
		g_AnimPattern[i].m_HPatternMax = h_patternMax;
		g_AnimPattern[i].m_seconds_per_pattern = seconds_per_pattern;
		g_AnimPattern[i].m_PatternSize = pattern_size;
		g_AnimPattern[i].m_StartPosition = start_position;
		g_AnimPattern[i].m_IsLooped = is_looped;
		
		return i;
	}
	return -1;
}

int SpriteAnim_CreatePlayer(int anim_pattern_id)
{
	for (int i = 0; i < ANIM_PLAY_MAX; i++) {
		if (g_AnimPlay[i].m_PatternId >= 0) continue;

		g_AnimPlay[i].m_PatternId = anim_pattern_id;
		g_AnimPlay[i].m_AccumulatedTime = 0.0;
		g_AnimPlay[i].m_PatternNum = 0;
		g_AnimPlay[i].m_IsStopped = false;

		return i;
	}
	return -1;
}

bool SpriteAnim_IsStopped(int index)
{
	return g_AnimPlay[index].m_IsStopped;
}

void SpriteAnim_DestroyPlayer(int index)
{
	g_AnimPlay[index].m_PatternId = -1;
}
