/*==============================================================================

	Map  [map.cpp]
														 Author : Zishan
														 Date   : 2025/07/16
--------------------------------------------------------------------------------

==============================================================================*/

#include "map.h"
#include "sprite.h"
#include "texture.h"
#include <DirectXMath.h>
using namespace DirectX;

static constexpr unsigned int MAPCHIP_WIDTH = 64;
static constexpr unsigned int MAPCHIP_HEIGHT = 64;
static constexpr unsigned int MAP_HORIZONTAL_COUNT = 50;
static constexpr unsigned int MAP_VERTICAL_COUNT = 15;
static constexpr unsigned int MAP_SCREEN_HORIZONTAL_COUNT = 26;
static constexpr unsigned int MAP_SCREEN_VERTICAL_COUNT = 16;

static int g_Map[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};
static int g_MapTexId = -1;

static XMFLOAT2 g_WorldOffset{ 0.0f,0.0f };

static void MapchipDraw(int mapChipId, int mx, int my, const XMFLOAT2& local_offset);

void Map_Initialize()
{
	g_MapTexId = Texture_Load(L"kokosozai.png");
}

void Map_Finalize()
{

}

void Map_Update(double elapsed_time)
{
}

void Map_Draw()
{
	const int Map_Vertical_Count = sizeof(g_Map) / sizeof(g_Map[0] / MAP_HORIZONTAL_COUNT);

	int mx = g_WorldOffset.x / MAPCHIP_WIDTH;
	int my = g_WorldOffset.y / MAPCHIP_HEIGHT;
	
	float local_offset_x = -fmodf(g_WorldOffset.x, MAPCHIP_WIDTH);
	float local_offset_y = -fmodf(g_WorldOffset.y, MAPCHIP_HEIGHT);

	local_offset_x = g_WorldOffset.x < 0 ? -(MAPCHIP_WIDTH - local_offset_x) : local_offset_x;
	local_offset_y = g_WorldOffset.y < 0 ? -(MAPCHIP_HEIGHT - local_offset_y) : local_offset_y;

	mx = g_WorldOffset.x < 0 ? mx - 1 : mx;
	my = g_WorldOffset.y < 0 ? my - 1 : my;

	for(int y = 0; y < MAP_SCREEN_VERTICAL_COUNT; y++){
		for (int x = 0; x < MAP_SCREEN_HORIZONTAL_COUNT; x++) {

			int offset_mapX = mx + x;
			int offset_mapY = my + y;

			if (offset_mapX < 0) offset_mapX = 0;
			if (offset_mapX >= MAP_HORIZONTAL_COUNT) offset_mapX = MAP_HORIZONTAL_COUNT - 1;

			if (offset_mapY < 0) offset_mapY = 0;
			if (offset_mapY >= MAP_VERTICAL_COUNT) offset_mapY = MAP_VERTICAL_COUNT - 1;

			const int index = offset_mapY * MAP_HORIZONTAL_COUNT + offset_mapX;

			if (g_Map[index] == 0) continue;

			MapchipDraw(g_Map[index], x, y, {local_offset_x,local_offset_y});
		}
	}
}

void Map_SetWorldOffset(const XMFLOAT2& world_offset)
{
	g_WorldOffset = world_offset;
}

bool Map_HitJudgementBoxVsMap(const Box& collision)
{
	int mx, my;

	mx = Map_GetWorldToMapX(collision.center.x - collision.half_width);
	my = Map_GetWorldToMapY(collision.center.y - collision.half_height);

	if (Map_GetMapChip(mx, my) > 0) {
		return true;
	}
	// Left Top
	mx = Map_GetWorldToMapX(collision.center.x + collision.half_width);
	my = Map_GetWorldToMapY(collision.center.y - collision.half_height);

	if (Map_GetMapChip(mx, my) > 0) {
		return true;
	}
	// Right Bottom
	mx = Map_GetWorldToMapX(collision.center.x - collision.half_width);
	my = Map_GetWorldToMapY(collision.center.y + collision.half_height);

	if (Map_GetMapChip(mx, my) > 0) {
		return true;
	}
	// Left bottom
	mx = Map_GetWorldToMapX(collision.center.x + collision.half_width);
	my = Map_GetWorldToMapY(collision.center.y + collision.half_height);

	if (Map_GetMapChip(mx, my) > 0) {
		return true;
	}
	return false;
}

int Map_GetWorldToMapX(float x)
{
	return (int)(x / MAPCHIP_WIDTH) + (x < 0 ? -1 : 0);
}

int Map_GetWorldToMapY(float y)
{
	return (int)(y / MAPCHIP_WIDTH) + (y < 0 ? -1 : 0);
}

int Map_GetMapChip(int mx, int my)
{
	if (mx < 0) mx = 0;
	if (mx >= MAP_HORIZONTAL_COUNT) mx = MAP_HORIZONTAL_COUNT - 1;
	if (my < 0) my = 0;
	if (my >= MAP_VERTICAL_COUNT) my = MAP_VERTICAL_COUNT - 1;

	return g_Map[my * MAP_HORIZONTAL_COUNT + mx];
}

void MapchipDraw(int mapChipId, int mx, int my, const XMFLOAT2& local_offset)
{
	if (mapChipId == 0) return;

	float px = (float) (mx * MAPCHIP_WIDTH) + local_offset.x;
	float py = (float)(my * MAPCHIP_HEIGHT) + local_offset.y;
											  
	Sprite_Draw(g_MapTexId, px, py, MAPCHIP_WIDTH, MAPCHIP_HEIGHT, 32 * (mapChipId - 1), 32 * 5, 32, 32);
}
