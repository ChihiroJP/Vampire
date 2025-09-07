/*==============================================================================

	Game –{‘Ì [game.cpp]
														 Author : Zishan
														 Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#include "game.h"
#include "Bg.h"
#include "player.h"
#include "runner.h"
#include "bullet.h"
#include "sprite.h"
#include "Enemy.h"
#include "enemy_spawner.h"
#include "effect.h"
#include "key_logger.h"
#include "Audio.h"
#include "score.h"
#include "direct3d.h"
#include "fade.h"
#include "texture.h"
#include "map.h"

static bool g_start = false;
void Hit_judgementBulletVsEnemy();
void Hit_judgementPlayerVsEnemy();

static int g_BgmId = -1;
static bool g_GameStart = false;

void Game_Initialize()
{
	Bg_Initialize();
	Map_Initialize();
	Player_Initialize({ 64.0f, 450 -32.0f });
	Runner_Initialize( { 64.0f,-100.0f} );
	Bullet_Initialize();
	Enemy_Initialize();
	Enemy_Spawner_Initialize();
	Effect_Initialize();

	// We get the screen width and subtract the total width of the score text.
	int score_digits = 7;
	float score_width = score_digits * 32.0f; // 7 digits * 32 pixels per digit
	float screen_width = (float)Direct3D_GetBackBufferWidth();
	Score_Initialize(screen_width - score_width - 10.0f, 10.0f, score_digits); // 10.0f is for padding

	Enemy_Spawner_Create({1600,128},			  ENEMY_TYPE_NORMAL, 3.0f, 0.3,5);
	Enemy_Spawner_Create({ 1600,900 - 128 - 64 }, ENEMY_TYPE_NORMAL, 5.0f, 0.3, 5);
	Enemy_Spawner_Create({ 1600,450- 64 },		  ENEMY_TYPE_2SHOT, 5.0f, 0.1, 5);
	Enemy_Spawner_Create({ 1600,450 - 64 },		  ENEMY_TYPE_2SHOT, 8.0f, 0.5, 3);
	
	g_BgmId = LoadAudio("audio2.wav");

	Fade_Start(1.0, false);
	g_GameStart = false;
}

void Game_Finalize()
{
	UnloadAudio(g_BgmId);

	Bg_Finalize();
	Score_Finalize();
	Effect_Finalize();
	Enemy_Spawner_Finalize();
	Enemy_Finalize();
	Bullet_Finalize();
	Runner_Finalize();
	Player_Finalize();
	Map_Finalize();
	Bg_Finalize();

	//Texture_A11Release();
}

void Game_Update(double elapsed_time)
{
	if (!g_GameStart && Fade_GetState() == FADE_STATE_FINISHED_IN) {
		PlayAudio(g_BgmId, true);
		g_GameStart = true;
	}
	if (KeyLogger_IsTrigger(KK_F)) {
		Fade_Start(1.0, true, { 1.0f,1.0f,1.0 });
	}
	if (g_GameStart && Fade_GetState() == FADE_STATE_FINISHED_OUT) {
		UnloadAudio(g_BgmId);
		g_GameStart = false;
	}

	Bg_Update(elapsed_time);
	Enemy_Spawner_Update(elapsed_time);
	Player_Update(elapsed_time);
	Runner_Update(elapsed_time);
	Bullet_Update(elapsed_time);
	Enemy_Update(elapsed_time);

	Hit_judgementBulletVsEnemy();
	Hit_judgementPlayerVsEnemy();

	Effect_Update(elapsed_time);
	Score_Update();
}

void Game_Draw()
{
	Bg_Draw();
	Map_Draw();
	Enemy_Draw();
	Bullet_Draw();
	 Player_Draw();
	Runner_Draw();
	Effect_Draw();
	Score_Draw();
}

void Hit_judgementBulletVsEnemy()
{
	for (int bi = 0; bi < BULLET_MAX; bi++) {

		if (!Bullet_IsEnable(bi)) continue;

		for (int ei = 0; ei < ENEMY_MAX; ei++) {

			if (!Enemy_IsEnable(ei)) continue;

			if (Collision_IsOverLapCircle(Bullet_GetCollision(bi), 
				Enemy_GetCollision(ei))) {

				BulletDestroy(bi);
				Enemy_Damage(ei);
				Score_AddScore(10);
			}
		}
	}
}

void Hit_judgementPlayerVsEnemy()
{
	if (!Player_IsEnable()) return;

	for (int ei = 0; ei < ENEMY_MAX; ei++) {

		if (!Enemy_IsEnable(ei)) continue;

		if (Collision_IsOverLapCircle(Player_GetCollision(),
			Enemy_GetCollision(ei))) {

			Player_Destroy();
			Enemy_Damage(ei);
			Effect_Create(Enemy_GetCollision(ei).center);
		}
	}
}
