/*=======================================================================================


Sprite_anim •\Ž¦ [sprite_anim.h]

																	Author : Zishan
																	Date	: 2025/6/17
-----------------------------------------------------------------------------------------
=========================================================================================*/

#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

#include <d3d11.h>
#include <DirectXMath.h>

void SpriteAnim_Initialize();
void SpriteAnim_Finalize();

void SpriteAnim_Update(double elapsed_time);
void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh);


int SpriteAnim_RegisterPattern(int textureId, int patternMax, int h_patternMax,double m_seconds_per_pattern,
	const DirectX::XMUINT2& pattern_size, const DirectX::XMUINT2& start_position, bool is_looped = true);

int SpriteAnim_CreatePlayer(int anim_pattern_id);
bool SpriteAnim_IsStopped(int index);
void SpriteAnim_DestroyPlayer(int index);

#endif // SPRITE_ANIM_H
