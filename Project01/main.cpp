#include <Windows.h>
#include <sstream>
#include "debug_ostream.h"
#include <SDKDDKVer.h>
#include "GameWindow.h"
#define WIN32_LEAN_AND_MEAN
#include "direct3d.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"
#include "sprite_anim.h"
#include "debug_text.h"
#include <sstream>
#include "system_timer.h"

// Main

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow)
{
	(void) CoInitializeEx(nullptr, COINIT_MULTITHREADED); // a yayy kyi loh m yayy loh m ya

	HWND hWnd = GameWindow_Create(hInstance);

	SystemTimer_Initialize();
	Direct3D_Initialize(hWnd);
	Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	SpriteAnim_Initialize();

	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetContext(),
		L"consolab_ascii_512.png",
		Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
		0.0f, 0.0f,
		0, 0,
		0.0f, 0.0f);

	int texid_zerotwo = Texture_Load(L"Zero two texture.png");
	int texid_my_wifey = Texture_Load(L"My_wife.png");
	int texid_kokosozai = Texture_Load(L"kokosozai.png");
	int texid_run01 = Texture_Load(L"runningman001.png");

	int aid_rw = SpriteAnim_RegisterPattern(texid_kokosozai, 13,13, 0.05, { 32,32 }, { 0,0 });
	int aid_lw = SpriteAnim_RegisterPattern(texid_kokosozai, 13,13, 0.1 ,{ 32, 32 }, { 0,32});
	int aid_sm = SpriteAnim_RegisterPattern(texid_kokosozai, 6,6, 0.1 ,{ 32,32 }, { 0,32*2 });
	int aid_head = SpriteAnim_RegisterPattern(texid_kokosozai, 6,6, 0.5, { 32,32 }, { 0,32 * 4 });
	int aid_to = SpriteAnim_RegisterPattern(texid_kokosozai, 4, 6,0.1 ,{ 32,32 }, {32*2,32*5 },false);
	int aid_run = SpriteAnim_RegisterPattern(texid_run01, 10, 5, 0.2, { 140, 200 }, { 0,0 }, true);

	int pid_01 = SpriteAnim_CreatePlayer(aid_rw);
	int pid_02 = SpriteAnim_CreatePlayer(aid_sm);
	int pid_03 = SpriteAnim_CreatePlayer(aid_to);
	int pid_04 = SpriteAnim_CreatePlayer(aid_lw);
	int pid_05 = SpriteAnim_CreatePlayer(aid_head);
	int pid_06 = SpriteAnim_CreatePlayer(aid_run);

	// position
	float position01_x = 512.0f;
	float position01_y = 64.0f;
	float position02_x = 1024.0f;
	float position02_y = 256.0f;

	// velocity
	float speed01_x = 100.0f;
	float speed01_y = 100.0f;



	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//Frame Count 
	double exec_last_time = SystemTimer_GetTime();
	double fps_last_time = exec_last_time;
	double current_time = 0.0;
	ULONG frame_count = 0;
	double current_fps = 0.0;

	/*Message Loop and Game Loop*/
	MSG msg;
	float x = 0.0f;

	do {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { // ウィンドウメッセージが来ていたら 
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else { 	//Game Logic

		// fps case
			current_time = SystemTimer_GetTime();
			double elapsed_time = current_time - fps_last_time;

			if (elapsed_time >= 1.0)
			{
				current_fps = frame_count / elapsed_time;
				fps_last_time = current_time;
				frame_count = 0;
			}
			// 1/60 秒ことに実行
			elapsed_time = current_time - exec_last_time;

			if (elapsed_time >= (1.0/60.0)) {
				exec_last_time = current_time;

				//Update Animation Frame
				SpriteAnim_Update(elapsed_time);

				position01_x += speed01_x * static_cast<float>(elapsed_time);
				position02_x -= speed01_y * static_cast<float>(elapsed_time);

				if (position01_x > 1280.0f) position01_x = -256.0f;
				if (position02_x <= 0.0f) position02_x = 1056.0f;
			}

			SpriteAnim_Update(elapsed_time);

			Direct3D_Clear();

			Sprite_Begin();

			Sprite_Draw(texid_zerotwo, 0.0f, 0.0f,512,512);
			Sprite_Draw(texid_my_wifey, 1000.0f, 0.0f,512,512);
						//Tex id ,		Left corner x, y, width, height

			SpriteAnim_Draw(pid_01, position01_x, position01_y, 256.0f, 256.0f);
			SpriteAnim_Draw(pid_02, 1024.0f, 64.0f, 256.0f, 256.0f);
			SpriteAnim_Draw(pid_03, 600.0f, 600.0f, 128.0f, 128.0f);
			SpriteAnim_Draw(pid_04, 1100.0f, 256.0f, 256.0f, 256.0f);
			SpriteAnim_Draw(pid_05, 512.0f, 256.0f, 256.0f, 256.0f);
			SpriteAnim_Draw(pid_06, position02_x ,position02_y, 256.0f, 256.0f);


#if defined(DEBUG) || defined(_DEBUG)
			std::stringstream ss;
			ss << "Fps: " << current_fps;
			dt.SetText(ss.str().c_str());

			/*dt.SetText("Zishan may be yours\n", { 0.0f, 512.0f, 0.0f, 1.0f });
			dt.SetText("Mukuro ", { 0.0f, 0.0f, 1.0f, 1.0f });
			dt.SetText("X chin dl ", { 0.0f, 0.0f, 1.0f, 1.0f });*/

			dt.Draw();
			dt.Clear();
#endif

			Direct3D_Present();

			frame_count++;
			//Sleep(17); // fps
		}

	} while (msg.message != WM_QUIT);

	SpriteAnim_Finalize();
	Texture_Finialize();
	Sprite_Finalize();
	Shader_Finalize();
	Direct3D_Finalize();

	CoUninitialize();

	return(int)msg.wParam;

}