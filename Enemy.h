/*==============================================================================

	Enemy  [enemy.h]
														 Author : Zishan
														 Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/

#ifndef ENEMY_H
#define ENEMY_H

#include <DirectXMath.h>
#include "collision.h"

static constexpr unsigned int ENEMY_MAX = 256; // Enemy

void Enemy_Initialize();
void Enemy_Finalize();

void Enemy_Update(double elapsed_time);
void Enemy_Draw();

enum EnemyTypeID : int
{
	ENEMY_TYPE_2SHOT,
	ENEMY_TYPE_NORMAL
};

void Enemy_Create(EnemyTypeID id ,const DirectX::XMFLOAT2& position);

bool Enemy_IsEnable(int index);
Circle Enemy_GetCollision(int index);

void EnemyDestroy(int index);

void Enemy_Damage(int index);

#endif // !ENEMY_H

