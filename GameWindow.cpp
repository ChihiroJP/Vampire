/*======================================================================================================================


	debug ostream を作って使おう　[debug_ostream.cpp]
																		Author : Youhei Sato
																		Date	: 2025/6/4
---------------------------------------------------------------------------------------------------------------------

	SHIFT_JISのみ対応
	====================================================================================================================*/
#include "GameWindow.h"
#include <algorithm>
#include "keyboard.h"
#include "mouse.h"

	/*--------------------------------------------------------------------------------------
			Window procedure prototype 宣言
		-------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*----------------------------------------------------------------------------------------
	Window Information
	---------------------------------------------------------------------------------------*/
static constexpr char WINDOW_CLASS[] = "GameWindow"; // main window access name
static constexpr char TITLE[] = "GameWindow"; //window のテキスト

HWND GameWindow_Create(HINSTANCE hInstance)
{
	/* Window クラスの登録 */
	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName = nullptr; // menu は作らない
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	/* Main Window の作成 */
	constexpr int SCREEN_WIDTH = 1600;
	constexpr int SCREEN_HEIGHT = 900;

	RECT window_rect{ 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };

	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	AdjustWindowRect(&window_rect, style, FALSE);

	const int WINDOW_WIDTH = window_rect.right - window_rect.left;
	const int WINDOW_HEIGHT = window_rect.bottom - window_rect.top;

	// primary monitor {window ko a lal mhr htrr nee)
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	const int WINDOW_X = std::max((desktop_width - WINDOW_WIDTH) / 2, 0);
	const int WINDOW_Y = std::max((desktop_height - WINDOW_HEIGHT) / 2, 0);
	HWND hWnd = CreateWindow(
		WINDOW_CLASS,
		TITLE,
		style,
		WINDOW_X,
		WINDOW_Y,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	return hWnd;
}

// window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	 case WM_ACTIVATEAPP:
		 Keyboard_ProcessMessage(message, wParam, lParam);
		 Mouse_ProcessMessage(message, wParam, lParam);
		 break;
     case WM_INPUT:
     case WM_MOUSEMOVE:
     case WM_LBUTTONDOWN:
     case WM_LBUTTONUP:
     case WM_RBUTTONDOWN:
     case WM_RBUTTONUP:
     case WM_MBUTTONDOWN:
     case WM_MBUTTONUP:
     case WM_MOUSEWHEEL:
     case WM_XBUTTONDOWN:
     case WM_XBUTTONUP:
     case WM_MOUSEHOVER:
         Mouse_ProcessMessage(message, wParam, lParam);
         break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	 case WM_SYSKEYUP:
		  Keyboard_ProcessMessage(message, wParam, lParam);
		         break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "Are u sure you want to close ?", "Comfirm", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY: // window message
		PostQuitMessage(0); // WM_QUIT message 送信
		break;

	default:
		// 通常の message 処理の関数に任せる
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}