/*==============================================================================

	Bullet  [bullet.cpp]
														 Author : Zishan
														 Date   : 2025/07/01
--------------------------------------------------------------------------------

==============================================================================*/

#include "bullet.h"
#include <DirectXMath.h>
#include "direct3d.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "Audio.h"
using namespace DirectX;

struct Bullet
{
	XMFLOAT2 position;
	XMFLOAT2 velocity;
	double bullet_life_time;
	bool isEnable;
	Circle collision;
};
static Bullet g_Bullets[BULLET_MAX]{};
static int g_BulletTextureId = -1;
static int g_BulletSoundId = -1;


void Bullet_Initialize()
{
	for (Bullet& b : g_Bullets) {
		b.isEnable = false;
	}
	g_BulletTextureId = Texture_Load(L"bullet1.png");
	g_BulletSoundId = LoadAudio("audio1.wav");
}

void Bullet_Finalize()
{
	UnloadAudio(g_BulletSoundId);
}

void Bullet_Update(double elapsed_time)
{
	for (Bullet& b : g_Bullets) {

		if (!b.isEnable) continue;

		XMVECTOR position = XMLoadFloat2(&b.position); // Float2 to XM Variable
		XMVECTOR velocity = XMLoadFloat2(&b.velocity);

		position += velocity * elapsed_time;

		XMStoreFloat2(&b.position, position);
		XMStoreFloat2(&b.velocity, velocity); 

		b.bullet_life_time += elapsed_time;
		if (b.bullet_life_time >= 5.0f) {
			b.isEnable = false;
		}
		if (b.position.x > Direct3D_GetBackBufferWidth()) {
			b.isEnable = false;
		}
	}
}

void Bullet_Draw()
{
	int i = 0;
	for (Bullet& b : g_Bullets) {
		i++;

		if (!b.isEnable) continue;

		Sprite_Draw(g_BulletTextureId, b.position.x, b.position.y, 
			0.0f, 0.0f,26,10);

#if defined(DEBUG) || defined(_DEBUG)
		Collision_DebugDraw(Bullet_GetCollision(i - 1));
#endif
	}
}

void Bullet_Shoot(const DirectX::XMFLOAT2& position)
{
	for (Bullet& b : g_Bullets) {

		if (b.isEnable) continue;

		b.isEnable = true;
		b.bullet_life_time = 0.0;
		b.position.x = position.x + (128.0f - 32.0f) / 2.0f;
		b.position.y = position.y + (128.0f - 32.0f) / 2.0f;
		b.velocity = { 400.0f,0.0f };
		b.collision = { { 13.0f, 5.0f},8.0f };

		PlayAudio(g_BulletSoundId, false);

		break;
	}
}

bool Bullet_IsEnable(int index)
{
	return g_Bullets[index].isEnable;
}

Circle Bullet_GetCollision(int index)
{
	float cx = g_Bullets[index].collision.center.x + g_Bullets[index].position.x;
	float cy = g_Bullets[index].collision.center.y + g_Bullets[index].position.y;

	return { { cx, cy }, g_Bullets[index].collision.radius };
}

void BulletDestroy(int index)
{
	g_Bullets[index].isEnable = false;
}
