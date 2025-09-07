/*==============================================================================

	Runner [runner.cpp]
														 Author : Zishan
														 Date   : 2025/07/18
--------------------------------------------------------------------------------

==============================================================================*/

#include "runner.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "texture.h"
#include "sprite.h"
#include "key_logger.h"
#include "bullet.h"
#include"direct3d.h"
#include "collision.h"
#include "map.h"

static constexpr float RUNNER_WIDTH = 64.0f;
static constexpr float RUNNER_HEIGHT = 64.0f;

static XMFLOAT2 g_RunnerPosition{};
static XMFLOAT2 g_RunnerVelocity{};
static int g_RunnerTexId = -1;
static constexpr Circle g_RunnerCollision{ {32.0f,32.0f},32.0f };

static bool g_RunnerEnable = true;
static bool g_IsJump = false;


void Runner_Initialize(const DirectX::XMFLOAT2& position)
{
	g_RunnerTexId = Texture_Load(L"kokosozai.png");
	g_RunnerPosition = position;
	g_RunnerVelocity = { 0.0f,0.0f };
	g_RunnerEnable = true;
}

void Runner_Finalize()
{
}

void Runner_Update(double elapsed_time)
{
	if (!g_RunnerEnable) return;

	XMVECTOR position = XMLoadFloat2(&g_RunnerPosition); // Float2 to XM Variable
	XMVECTOR velocity = XMLoadFloat2(&g_RunnerVelocity);

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

	if (KeyLogger_IsTrigger(KK_J) && !g_IsJump) {
		XMFLOAT2 jump {0.0f,-50000.0f * (float)elapsed_time} ;
		velocity += XMLoadFloat2(&jump);
		g_IsJump = true;
	}

	//Gravity
	XMFLOAT2 gdir{ 0.0f, 1.0f };
	velocity += XMLoadFloat2(&gdir) * 9.8f * 100.0f * (float)elapsed_time;
	position += velocity * (float)elapsed_time;
	DirectX::XMStoreFloat2(&g_RunnerPosition, position);

	// elapsed_time = the time, in seconds, that has passed since the last frame. 

	if (Map_HitJudgementBoxVsMap(Runner_GetBoxCollision())) {
		position -= velocity * (float)elapsed_time;
		DirectX::XMStoreFloat2(&g_RunnerPosition, position);
		velocity *= { 1.0f, 0.0f };
		g_IsJump = false;
	}
	// Left Right movement
	direction = XMVector2Normalize(direction);
	velocity += direction * (float)3000000.0f / 2500.0f * elapsed_time;
	XMFLOAT2 right_dir{ 1.0f,0.0f };
	velocity += XMLoadFloat2(&right_dir) * -XMVectorGetX(velocity) * (float)(4.0 * elapsed_time);
	position += velocity * (float)elapsed_time;
	DirectX::XMStoreFloat2(&g_RunnerPosition, position);

	if (Map_HitJudgementBoxVsMap(Runner_GetBoxCollision())) {
		position -= velocity * (float)elapsed_time;
		DirectX::XMStoreFloat2(&g_RunnerPosition, position);
		velocity *= { 0.0f, 1.0f };
		//g_IsJump = false;
	}

	DirectX::XMStoreFloat2(&g_RunnerVelocity, velocity);// XM Variable to Float2

	XMFLOAT2 offset;
	offset.x = g_RunnerPosition.x - (Direct3D_GetBackBufferWidth() - RUNNER_WIDTH) * 0.5f;
	offset.y = g_RunnerPosition.y - RUNNER_HEIGHT * 10.0f;

	Map_SetWorldOffset(offset);

	if (KeyLogger_IsTrigger(KK_SPACE)) {
		float x = (Direct3D_GetBackBufferWidth() - RUNNER_WIDTH) * 0.5f;
		float y = RUNNER_HEIGHT * 10.0f;

		Bullet_Shoot({ x, y});
	}
}

void Runner_Draw()
{
	float x = (Direct3D_GetBackBufferWidth() - RUNNER_WIDTH) * 0.5f;
	float y = RUNNER_HEIGHT * 10.0f; //Direct3D_GetBackBufferHeight() - RUNNER_HEIGHT * 2.0f;
	Sprite_Draw(g_RunnerTexId, x, y, RUNNER_WIDTH, RUNNER_HEIGHT, 0, 0, 32, 32);
}

bool Runner_IsEnable()
{
	return g_RunnerEnable;
}

Circle Runner_GetCircleCollision()
{
	return Circle();
}

Box Runner_GetBoxCollision()
{
	float hw = RUNNER_WIDTH / 2;
	float hh = RUNNER_HEIGHT / 2;

	return {{ g_RunnerPosition.x + hw, g_RunnerPosition.y + hh }, hw, hh};
}

void Runner_Destroy()
{
	g_RunnerEnable = false;
}
