#pragma once
#include <Windows.h>
#define POLMOD_VERSION "0.3"


namespace MainHooks {
	LRESULT CALLBACK HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void HookWndProc();
}

extern int iLastKey;