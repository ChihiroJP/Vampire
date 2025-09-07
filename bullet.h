/*==============================================================================

	Bullet  [bullet.h]
														 Author : Zishan
														 Date   : 2025/07/01
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef BULLET_H
#define BULLET_H

#include <DirectXMath.h>
#include "collision.h"

static constexpr unsigned int BULLET_MAX = 1024;

void Bullet_Initialize();
void Bullet_Finalize();

void Bullet_Update(double elapsed_time); 
void Bullet_Draw();

void Bullet_Shoot(const DirectX::XMFLOAT2& position);

bool Bullet_IsEnable(int index);
Circle Bullet_GetCollision(int index);

void BulletDestroy(int index);

#endif // !BULLET_H
