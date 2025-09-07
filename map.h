/*==============================================================================

	Map  [map.h]
														 Author : Zishan
														 Date   : 2025/07/16
--------------------------------------------------------------------------------

==============================================================================*/#pragma once
#ifndef MAP_H
#define MAP_H

#include <DirectXMath.h>
#include "collision.h"

void Map_Initialize();
void Map_Finalize();
void Map_Update(double elapsed_time);
void Map_Draw();

void Map_SetWorldOffset(const DirectX::XMFLOAT2& world_offset);

bool Map_HitJudgementBoxVsMap(const Box& collision);

int Map_GetWorldToMapX(float x);
int Map_GetWorldToMapY(float y);

int Map_GetMapChip(int mx, int my);

#endif // MAP_H