/*==============================================================================

Title[title.cpp]
														 Author : Zishan
														 Date   : 2025/07/11
--------------------------------------------------------------------------------

==============================================================================*/

#include "title.h"
#include "texture.h"
#include "sprite.h"
#include "key_logger.h"
#include "scene.h"
#include "fade.h"
#include "game.h"

static int g_TitleBgTexId = -1;
static Scene g_Scene = SCENE_GAME;
static Scene g_SceneNext = g_Scene;

enum TitleStart
{
	TITLE_STATE_NONE,
	TITLE_STATE_FADE_IN,
	TITLE_STATE_FIGHTER_SHOW,
	TITLE_STATE_LOGO_SHOW,
	TITLE_STATE_KEY_WAIT,
	TITLE_STATE_FADE_OUT,
	TITLE_STATE_MAX
};

static TitleStart g_State = TITLE_STATE_NONE;

void Title_Initialize()
{
	g_TitleBgTexId = Texture_Load(L"GameTitle.png");
}

void Title_Finalize()
{
	//exture_A11Release();
}

void Title_Update(double elapsed_time)
{
	
	/*switch (g_State)
	{
	case TITLE_STATE_FADE_IN:
		if (Fade_GetState() == FADE_STATE_FINISHED_IN) {
			g_State = TITLE_STATE_FIGHTER_SHOW;
		}
		break;
	case TITLE_STATE_FIGHTER_SHOW:
		g_FighterAlpha = std :: min(( float)(g_AccumulatedTime - g_ShowStartTiime))
	}*/

	if (KeyLogger_IsTrigger(KK_ENTER)) {
		Fade_Start(1.0f, true);
	}
	if (Fade_GetState() == FADE_STATE_FINISHED_OUT) {
		Scene_Change(SCENE_GAME);
	}
}

void Title_Draw()
{
	Sprite_Draw(g_TitleBgTexId, 0.0f, 0.0f);
}
