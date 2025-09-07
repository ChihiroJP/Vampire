/*==============================================================================

	Runner [runner.h]
														 Author : Zishan
														 Date   : 2025/07/18
--------------------------------------------------------------------------------

==============================================================================*/#pragma once
#ifndef RUNNER_H
#define RUNNER_H

#include <DirectXMath.h>
#include "collision.h"

void Runner_Initialize(const DirectX::XMFLOAT2& position);
void Runner_Finalize();
	 
void Runner_Update(double elapsed_time);
void Runner_Draw();

bool Runner_IsEnable();
Circle Runner_GetCircleCollision();
Box Runner_GetBoxCollision();
void Runner_Destroy();

#endif // RUNNER_H
