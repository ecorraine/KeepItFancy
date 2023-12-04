#ifndef KEYINPUT_H
#define KEYINPUT_H

#include <Windows.h>
#include <cstdlib>

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! KEYINPUT Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class KEYINPUT KeyInput.h "KeyInput.h"
/*! \brief Keyboard Input Class
 *  \brief キーボード入力クラス
 */
class KEYINPUT
{
public:
	static void InitKeyInput();
	static void UpdateKeyInput();
	static void ReleaseKeyInput() {}

	static bool KeyPress(BYTE vkey);
	static bool KeyTrigger(BYTE vkey);
	static bool KeyRelease(BYTE vkey);

private:
	static BYTE g_KeyState[256];	// 全キー状態を保存する配列
	static BYTE g_OldKeyState[256];	// 全キー状態（１フレーム前）を保存する配列
};

#endif // !KEYINPUT_H

