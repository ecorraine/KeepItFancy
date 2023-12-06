#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include "defines.h"

const auto CLASS_NAME = TEXT("DX11 | [2023]Keep It Fancy");
const auto WINDOW_NAME = TEXT("DX11 | [2023]Keep It Fancy");

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! APPLICATION Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class APPLICATION Application.h "Application.h"
/*! \brief Handler class.
 *  \brief ハンドラークラス
 */
class APPLICATION
{
public:
	APPLICATION(uint32_t screenWidth, uint32_t screenHeight) :
		m_hInst(nullptr),
		m_hWnd(nullptr),
		m_Width(screenWidth),
		m_Height(screenHeight)
	{}
	~APPLICATION() {}

	void Boot();

	uint32_t		GetScreenWidth() { return m_Width; }
	uint32_t		GetScreenHeight() { return m_Height; }

	int	MainLoop();
	HWND			GetWindow() { return m_hWnd; }

private:
	uint32_t		m_Width;
	uint32_t		m_Height;

	HINSTANCE		m_hInst;
	HWND			m_hWnd;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Shutdown();
	void TermWindow();

private:
	bool Startup();
	bool InitWindow();
};

#endif // !APPLICATION_H

