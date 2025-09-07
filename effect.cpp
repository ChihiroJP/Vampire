/*==============================================================================

	Effect  [effect.cpp]
														 Author : CheeHow
														 Date   : 2025/07/04
--------------------------------------------------------------------------------

==============================================================================*/

#include "effect.h"
#include <DirectXMath.h>
#include "texture.h"
#include "sprite_anim.h"
using namespace DirectX;
#include "Audio.h"

struct Effect
{
	XMFLOAT2 position;
	//XMFLOAT2 velocity;
	//double effect_life_time;
	int sprite_anim_id;
	bool isEnable;
};

static constexpr int EFFECT_MAX = 256;
static Effect g_Effect[EFFECT_MAX]{};
static int g_EffectTextureId = -1;
static int g_AnimPatternId = -1;
static int g_EffectSoundId = -1;

void Effect_Initialize()	
{
	for (Effect& e : g_Effect) {
		e.isEnable = false;
	}
	g_EffectTextureId = Texture_Load(L"Explosion.png");
	g_EffectSoundId = LoadAudio("audio1.wav");
	g_AnimPatternId = SpriteAnim_RegisterPattern(g_EffectTextureId, 16, 4, 0.01, { 256, 256 }, { 0,0 }, false);
}

void Effect_Finalize()
{
	UnloadAudio(g_EffectSoundId);
}

void Effect_Update(double elapsed_time)
{
	for (Effect& e : g_Effect) {
		if (!e.isEnable) continue;

		if (SpriteAnim_IsStopped(e.sprite_anim_id)) {
			SpriteAnim_DestroyPlayer(e.sprite_anim_id);

			e.isEnable = false;
		}
	}
}

void Effect_Draw()
{
	for (Effect& e : g_Effect) {

		if (!e.isEnable) continue;

		SpriteAnim_Draw(e.sprite_anim_id, e.position.x, e.position.y, 64.0f, 64.0f);
	}
}

void Effect_Create(const DirectX::XMFLOAT2& position)
{
	for (Effect& e : g_Effect) {

		if (e.isEnable) continue;

		e.isEnable = true;
		e.position = position;
		e.sprite_anim_id = SpriteAnim_CreatePlayer(g_AnimPatternId);
		PlayAudio(g_EffectSoundId);

		break;
	}
}