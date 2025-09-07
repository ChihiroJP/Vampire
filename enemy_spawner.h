/*==============================================================================

	Enemy Spawner  [enemy_spawner.h]
														 Author : Zishan
														 Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/

#ifndef enemy_spawner
#define ENEMY_SPAWNER
#include "Enemy.h"

#include <DirectXMath.h>


void Enemy_Spawner_Initialize();
void Enemy_Spawner_Finalize();

void Enemy_Spawner_Update(double elapsed_time);

void Enemy_Spawner_Create(const DirectX::XMFLOAT2& position, EnemyTypeID id, 
	double spawn_time,double spawn_rate, int spawn_count );

#endif // enemy_spawner

