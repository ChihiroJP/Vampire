/*==============================================================================

	Collision  [collision.h]
														 Author : Zishan
														 Date   : 2025/07/03
--------------------------------------------------------------------------------

==============================================================================*/#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include "d3d11.h"
#include <DirectXMath.h>

extern ID3D11Device* g_pDevice;
extern ID3D11DeviceContext* g_pContext;

struct Circle
{
	DirectX::XMFLOAT2 center;
	float radius;
};

struct Box
{
	DirectX::XMFLOAT2 center;
	float half_width;
	float half_height;
};

bool Collision_IsOverLapCircle(const Circle& a, const Circle& b);
bool Colllision_IsOverLapBox(const Box& a, const Box& b);

void Collision_DebugInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Collision_DebugFinalize();
void Collision_DebugDraw(const Circle& circle,const DirectX::XMFLOAT4& color = { 1.0f,1.0f,0.0f,1.0f});
void Collision_DebugDraw(const Box& box, const DirectX::XMFLOAT4& color = { 1.0f,1.0f,0.0f,1.0f });

#endif // !COLLISION_H

