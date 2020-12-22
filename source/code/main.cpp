#include "polmod.h"
#include "poldekmp.h"
#include "main.h"
#include <iostream>
WNDPROC wOriginalWndProc = nullptr;
int iLastKey = 0;

LRESULT CALLBACK MainHooks::HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_F1)
			PolMod::bDebugMenu ^= 1;

		iLastKey = wParam;
		break;

	default:
		break;
	}


	return ((LRESULT(__stdcall*)(HWND,UINT,WPARAM,LPARAM))0x41AB10)(hWnd,uMsg,wParam,lParam);
}

void MainHooks::HookWndProc()
{
	// okno za pozno sie robi, wiec hookowane jest pod funkcje
	HWND window = FindWindow(0, "Poldek");
	wOriginalWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
}
