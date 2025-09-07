/*==============================================================================

	Player [player.cpp]
														 Author : Zishan
														 Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/

#include "player.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "texture.h"
#include "sprite.h"
#include "key_logger.h"
#include "bullet.h"
#include"direct3d.h"
#include "collision.h"

static constexpr float Player_Width = 128.0f;
static constexpr float Player_Height = 128.0f;

static XMFLOAT2 g_PlayerPosition{};
static XMFLOAT2 g_PlayerVelocity{};
static int g_PlayerTextureId {-1};
static Circle g_PlayerCollision{ {64.0f,64.0f},48.0f };
static bool g_PlayerEnable = true;

void Player_Initialize(const XMFLOAT2& position)
{
	g_PlayerPosition = position;
	g_PlayerVelocity = { 0.0f, 0.0f };
	g_PlayerEnable = true;
	g_PlayerTextureId = Texture_Load(L"Player_Spaceship.png");
}

void Player_Finalize()
{
}

void Player_Update(double elapsed_time)
{
	if (!g_PlayerEnable) return;

	XMVECTOR position = XMLoadFloat2(&g_PlayerPosition); // Float2 to XM Variable
	XMVECTOR velocity = XMLoadFloat2(&g_PlayerVelocity);
	XMVECTOR direction{};

	if (KeyLogger_IsPressed(KK_W)) {
		direction += {0.0f, -1.0f};
	}

	if (KeyLogger_IsPressed(KK_A)) {
		direction += {-1.0f, 0.0f};
	}

	if (KeyLogger_IsPressed(KK_S)) {
		direction += {0.0f, 1.0f};
	}

	if (KeyLogger_IsPressed(KK_D)) {
		direction += {1.0f, 0.0f};
	}

	velocity += direction * 6000000.0f / 2500.0f * elapsed_time; // V1 = V0 + acceleration * Deltat
	position += velocity * elapsed_time;						// P1 = P0 + velocity * Deltat
	velocity += -velocity * 4.0f * elapsed_time;

	// elapsed_time = the time, in seconds, that has passed since the last frame. 

	XMStoreFloat2(&g_PlayerPosition, position);
	XMStoreFloat2(&g_PlayerVelocity, velocity);// XM Variable to Float2

	//int P_screenWidth = Direct3D_GetBackBufferWidth();
	//int P_screenHeight = Direct3D_GetBackBufferHeight();

	if (KeyLogger_IsTrigger(KK_SPACE)) {
		//Bullet_Shoot(XMFLOAT2{ g_PlayerPosition.x + 16, g_PlayerPosition.y +16 });
	}
}

void Player_Draw()
{
	if (!g_PlayerEnable) return;

	Sprite_Draw(g_PlayerTextureId, 
		//g_PlayerPosition.x, g_PlayerPosition.y,
		800 -32,450,
		Player_Width, Player_Height, 0, 0, 1024, 542);

#if defined(DEBUG) || defined(_DEBUG)
	Collision_DebugDraw(Player_GetCollision());
#endif

	Box box{{ g_PlayerPosition.x, g_PlayerPosition.y }, Player_Width, Player_Height};
	Collision_DebugDraw(box);
}

bool Player_IsEnable()
{
	return g_PlayerEnable;
}

Circle Player_GetCollision()
{
	float cx = g_PlayerPosition.x + g_PlayerCollision.center.x;
	float cy = g_PlayerPosition.y + g_PlayerCollision.center.y;

	return { { cx, cy }, g_PlayerCollision.radius};
}

void Player_Destroy()
{
	g_PlayerEnable = false;
}
