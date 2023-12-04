#include "KeyInput.h"

BYTE KEYINPUT::g_KeyState[256];
BYTE KEYINPUT::g_OldKeyState[256];

void KEYINPUT::InitKeyInput()
{
	memset(g_OldKeyState, 0, 256);
	memset(g_KeyState, 0, 256);
}

void KEYINPUT::UpdateKeyInput()
{
	// ゲームループの最後で現在のキー状態をOldKeyStateに保存
	memcpy_s(g_OldKeyState, sizeof(g_OldKeyState), g_KeyState, sizeof(g_KeyState));

	GetKeyboardState(g_KeyState);
}

bool KEYINPUT::KeyPress(BYTE vkey)
{
	return (g_KeyState[vkey] & 0x80);
}

bool KEYINPUT::KeyTrigger(BYTE vkey)
{
	return (g_KeyState[vkey] ^ g_OldKeyState[vkey]) & g_KeyState[vkey] & 0x80;
}

bool KEYINPUT::KeyRelease(BYTE vkey)
{
	return (g_KeyState[vkey] ^ g_OldKeyState[vkey]) & g_KeyState[vkey] & 0x80;
}
