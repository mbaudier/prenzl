#include "stdafx.h"				

#include "PrenzlComputation.h"
#include "PrenzlConfDialog.h"

using namespace Prenzl;

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	INITCOMMONCONTROLSEX icx;
	icx.dwSize = sizeof(INITCOMMONCONTROLSEX);	// couldn't they have picked a shorter struct name...?
	icx.dwICC = ICC_WIN95_CLASSES;				// just want classes for the slider
	InitCommonControlsEx(&icx);					// the hoops we have to jump through...
    return TRUE;								// all done here...
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	static PrenzlConfDialog mainDialog;
	return mainDialog.processMessage(hDlg, message, wParam, lParam);
}

LONG WINAPI ScreenSaverProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static PrenzlComputation * computation = NULL;
	RECT rc;
	HDC hdc;

	switch(message) {
	case WM_CREATE:
		computation = new PrenzlComputation(hwnd);			// config gets read here too
		break;

	case WM_ERASEBKGND:
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rc);
		FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
		ReleaseDC(hwnd, hdc);
		break;

	case WM_TIMER:
		hdc = GetDC(hwnd);		// do this in the timer just in case the user somehow
								// changes pixel depths while the screen saver is running
								// (tell me how!)
								// still a big benefit doing it here instead of the inner loop
		if(computation) {
			computation->paint(hdc);
		}

		ReleaseDC(hwnd, hdc);	// :-)
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		delete computation;
		computation = NULL;
		break;
	}
    return DefScreenSaverProc(hwnd, message, wParam, lParam); 
}

