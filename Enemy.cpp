/*==============================================================================

	Enemy  [enemy.cpp]
														 Author : CheeHow
														 Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/

#include "Enemy.h"
#include <DirectXMath.h>
#include "direct3d.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "effect.h"
#include "score.h"

using namespace DirectX;

struct EnemyType
{
	int TextureId;
	int tx, ty; // Texture x ,y
	int tw, th; // Texture width , height
	XMFLOAT2 velocity;
	Circle collision;
	int hp;
	unsigned int score;

};

struct Enemy
{
	int typeId;
	int hp;
	double LifeTime;
	float offsetY;
	XMFLOAT2 position;
	bool isEnable;
	bool isDamage;
};

static Enemy g_Enemy[ENEMY_MAX]{};
static int g_EnemyTextureId = -1;
static constexpr float ENEMY_WIDTH = 64.0f;

static EnemyType g_EnemyType[]{
{ -1, 64 , 0, 2048, 2048, {-200.0f,0.0f},{{32.0f,32.0f},32.0f },1},
{ -1, 0, 32, 2048, 2048,{-400.0f,0.0f}, {{32.0f,32.0f},32.0f },1} 
};

void Enemy_Initialize()
{
	for (Enemy& e : g_Enemy) {
		e.isEnable = false;
	}
	int texId = Texture_Load(L"../../Enemy1.png");
	int texId1 = Texture_Load(L"../../Enemy.png");
	g_EnemyType[0].TextureId = texId;
	g_EnemyType[1].TextureId = texId1;
}

void Enemy_Finalize()
{
}

void Enemy_Update(double elapsed_time)
{
	for (Enemy& e : g_Enemy) {

		if (!e.isEnable) continue;

		switch (e.typeId)
		{ 
		case ENEMY_TYPE_NORMAL:

			XMVECTOR position = XMLoadFloat2(&e.position); // Float2 to XM Variable
			XMVECTOR velocity = XMLoadFloat2(&g_EnemyType[e.typeId].velocity);

			position += velocity * elapsed_time;

			XMStoreFloat2(&e.position, position);
			break;

		case ENEMY_TYPE_2SHOT:

			e.position.x += g_EnemyType[e.typeId].velocity.x * elapsed_time;
			e.position.y = e.offsetY + sin(e.LifeTime * 3.0	) * 120.0f;
			break;
	}
	e.LifeTime += elapsed_time;

		if (e.position.x + ENEMY_WIDTH < 0.0f) {
			e.isEnable = false;
		}
	}
}

void Enemy_Draw()
{
	Direct3D_SetAlphaBlendAdd();

	int i = 0;
	for (Enemy& e : g_Enemy) {
		i++;
		if (!e.isEnable) continue;

		const EnemyType& type = g_EnemyType[e.typeId];
		Sprite_Draw(type.TextureId,
			e.position.x, e.position.y,
			ENEMY_WIDTH, 64.0f,
			type.tx, type.ty,
			type.tw, type.th,
			e.isDamage ? XMFLOAT4{1.0f, 1.0f,0.0f,1.0f} : XMFLOAT4{1.0f,1.0f,1.0f,1.0f});

		e.isDamage = false;

#if defined(DEBUG) || defined(_DEBUG)
		Collision_DebugDraw(Enemy_GetCollision(i - 1));
#endif
	}
	Direct3D_SetAlphaBlendTransparent();
}

void Enemy_Create(EnemyTypeID id, const DirectX::XMFLOAT2& position)
{
	for (Enemy& e : g_Enemy) {

		if (e.isEnable) continue;

		e.isEnable = true;
		e.typeId = id;
		e.offsetY = position.y;
		e.position = position;
		g_EnemyType[e.typeId].velocity = { -200.0f,0.0f };
		e.LifeTime = 0.0;
		e.hp = g_EnemyType[e.typeId].hp;
		e.isDamage = false;

		break;
	}
}

bool Enemy_IsEnable(int index)
{
	return g_Enemy[index].isEnable;
}

Circle Enemy_GetCollision(int index)
{
	int id = g_Enemy[index].typeId;

	float cx = g_Enemy[index].position.x + g_EnemyType[id].collision.center.x;
	float cy = g_Enemy[index].position.y + g_EnemyType[id].collision.center.y;

	return { { cx, cy }, g_EnemyType[id].collision.radius };
}

void EnemyDestroy(int index)
{
	g_Enemy[index].isEnable = false;
}

void Enemy_Damage(int index)
{
	g_Enemy[index].hp--;
	g_Enemy[index].isDamage = true;

	if (g_Enemy[index].hp <= 0) {
		Effect_Create(g_Enemy[index].position);
		g_Enemy[index].isEnable = false;
		//Score_AddScore(10);
	}
}
