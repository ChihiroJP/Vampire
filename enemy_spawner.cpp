/*==============================================================================

	Enemy Spawner  [enemy_spawner.cpp]
														 Author : Zishan
														 Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/

#include "enemy_spawner.h"
#include "Enemy.h"
#include<DirectXMath.h>

using namespace DirectX;

struct Enemy_Spawn
{
	XMFLOAT2 position;
	EnemyTypeID id;
	double time;
	double rate; 
	int count;
	double spawn_time;
	int spawn_count;
	bool isOver;
};

static constexpr unsigned int ENEMY_SPAWNERMAX = 100;
static Enemy_Spawn g_EnemySpawner[ENEMY_SPAWNERMAX]{};
static int g_SpawnerCount = 0;
static double g_time = 0.0;

void Enemy_Spawner_Initialize()
{
	g_SpawnerCount = 0;
	g_time = 0.0;
}

void Enemy_Spawner_Finalize()
{
}

void Enemy_Spawner_Update(double elapsed_time)
{
	g_time += elapsed_time;
	
	for (int i = 0; i < g_SpawnerCount; i++) {

		if (g_EnemySpawner[i].isOver) continue;

		if (g_EnemySpawner[i].time > g_time) break;

		if (g_EnemySpawner[i].spawn_count == 0) {
			g_EnemySpawner[i].spawn_time = g_time + g_EnemySpawner[i].rate - 0.0001;
		}

		if (g_EnemySpawner[i].spawn_time >= g_EnemySpawner[i].rate) {
			Enemy_Create(g_EnemySpawner[i].id, g_EnemySpawner[i].position);
			g_EnemySpawner[i].spawn_count++;

			if (g_EnemySpawner[i].count != -1 && g_EnemySpawner[i].spawn_count >= g_EnemySpawner[i].count) {
				g_EnemySpawner[i].isOver = true;
			}
			g_EnemySpawner[i].spawn_time = g_time;
		}
	}
}
void Enemy_Spawner_Create(const DirectX::XMFLOAT2& position, EnemyTypeID id, double spawn_time, 
	double spawn_rate, int spawn_count)
{
	if (g_SpawnerCount >= ENEMY_SPAWNERMAX)return;

	Enemy_Spawn* pEs = &g_EnemySpawner[g_SpawnerCount];
	pEs->position = position;
	pEs->id = id;
	pEs->time = spawn_time;
	pEs->rate = spawn_rate;
	pEs->count = spawn_count;
	pEs->spawn_count = 0;
	pEs->isOver = false;
	pEs->spawn_time = 0.0;
	g_SpawnerCount++;
}
