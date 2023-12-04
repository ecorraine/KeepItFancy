#include "Application.h"
#include "Game.h"

void APPLICATION::Shutdown()
{
	GAME::ReleaseGame();

	TermWindow();
}

void APPLICATION::TermWindow()
{
	if (m_hInst != nullptr)
		UnregisterClass(CLASS_NAME, m_hInst);

	m_hInst = nullptr;
	m_hWnd = nullptr;
}

int APPLICATION::MainLoop()
{
	MSG msg = {};
	ZeroMemory(&msg, sizeof(msg));

	uint64_t nowTime = 0;
	uint64_t lastTime = 0;

	timeBeginPeriod(1);
	lastTime = timeGetTime();

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			nowTime = timeGetTime();
			g_DeltaTime = (nowTime - lastTime);

			// 1/60秒判定
			if (g_DeltaTime >= 1000.0f / 60)
			{
				g_Tick = (float)(g_DeltaTime * 0.001f);

				GAME::UpdateGame(g_Tick);
				GAME::DrawGame();

				lastTime = nowTime;
			}
		}
		timeEndPeriod(1);
	}

	Shutdown();

	return (int)msg.wParam;
}

void APPLICATION::Boot()
{
	if (!Startup())
		MessageBoxA(GetWindow(), "Failed to boot application!", "ERROR", MB_OK | MB_ICONERROR);
}

bool APPLICATION::Startup()
{
	if (!InitWindow())
		return false;

	GAME::InitGame(this);

	return true;
}

bool APPLICATION::InitWindow()
{
	auto hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr)
		return false;

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	//wc.cbClsExtra		= 0;
	//wc.cbWndExtra		= 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

	// register window class
	if (!RegisterClassEx(&wc))
		return false;

	m_hInst = hInst;

	// set window size
	RECT rect = {};
	rect.right = static_cast<LONG>(m_Width);
	rect.bottom = static_cast<LONG>(m_Height);
	// adjust window
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, style, FALSE);

	//----- Create Window -----//
	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		style,							// ウィンドウスタイル
		CW_USEDEFAULT,					// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,					// ウィンドウの左上Ｙ座標
		(rect.right - rect.left),		// ウィンドウの幅
		(rect.bottom - rect.top),		// ウィンドウの高さ
		nullptr,
		nullptr,
		m_hInst,
		nullptr);

	if (m_hWnd == nullptr)
		return false;

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
	SetFocus(m_hWnd);

	return true;
}

LRESULT APPLICATION::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//case WM_KEYDOWN:
	//	switch (wParam)
	//	{
	//	case VK_ESCAPE:
	//	{
	//		int msgBox = MessageBoxA(hWnd, "Exit Application?\n終了しますか?", "Exit", MB_YESNO | MB_ICONINFORMATION);
	//		if (msgBox == IDYES)
	//			PostMessage(hWnd, WM_CLOSE, wParam, lParam);
	//	}
	//	break;
	//	}
	//case WM_KEYUP:
	//	break;

	//case WM_CLOSE:
	//	DestroyWindow(hWnd);
	//	break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}
