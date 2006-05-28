/* xscreensaver, Copyright (c) 1999 Jamie Zawinski <jwz@jwz.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 *
 * Matrix -- simulate the text scrolls from the movie "The Matrix".
 *
 * The movie people distribute their own Windows/Mac screensaver that does
 * a similar thing, so I wrote one for Unix.  However, that version (the
 * Windows/Mac version at http://www.whatisthematrix.com/) doesn't match my
 * memory of what the screens in the movie looked like, so my `xmatrix'
 * does things differently.
 *
 * ----------------------------------------------------------------------------
 *
 * Windows port by Craig Boston.  I liked the X screensaver better than the
 * "official" one so I decided to steal the code and convert it to a Win32
 * screensaver to use on my NT (actually Win2k) box at work.  Plus I wanted
 * multimonitor support so it would REALLY be like in the movie :-)
 *
 * The original code is mostly intact; only the X drawing/window code has been
 * removed and replaced with Win32 GDI calls.  Anything else that I changed I
 * tried to include an option to turn it on or off in the configuration dialog.
 *
 * I think jwz's comments are all C-style (/ * * /), whereas I used C++
 * style ( // ) wherever possible so the difference would be noticible.
 */

#include "stdafx.h"				
#include <Shlobj.h>

#include "PrenzlConfig.h"
#include "PrenzlComputation.h"
#include "Model/Util.h"

//extern HINSTANCE hMainInstance;	// screen saver instance handle

using namespace Prenzl;

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	INITCOMMONCONTROLSEX icx;
	icx.dwSize = sizeof(INITCOMMONCONTROLSEX);	// couldn't they have picked a shorter struct name...?
	icx.dwICC = ICC_WIN95_CLASSES;				// just want classes for the slider
	InitCommonControlsEx(&icx);					// the hoops we have to jump through...
    return TRUE;								// all done here...
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static HWND hSpeed;
	static HWND hDelay;
	static HWND hRuleCombo;
	static HWND hVDIVCombo;
	static HWND hHDIVCombo;
	static HWND hPositionCombo;
	static HWND hPropButton;
	static HWND hRestartCheck;
	static HWND hRestartValue;
	static HWND hClipBoard;
	static HWND hInputScreenRadio;
	static HWND hInputFilesRadio;
	static HWND hFilesDirectory;
	static HWND hBrowseButton;

	static PrenzlConfig config;

	switch(message) {
	case WM_INITDIALOG:

		hSpeed = GetDlgItem(hDlg, IDC_SPEED);
		hDelay = GetDlgItem(hDlg, IDC_DELAY);
		hRuleCombo = GetDlgItem(hDlg, IDC_RULECOMBO);
		hVDIVCombo = GetDlgItem(hDlg, IDC_VDIVCOMBO);
		hHDIVCombo = GetDlgItem(hDlg, IDC_HDIVCOMBO);
		hPositionCombo = GetDlgItem(hDlg, IDC_POSITION_COMBO);
		hPropButton = GetDlgItem(hDlg, IDC_PROPBUTTON);
		hRestartCheck = GetDlgItem(hDlg, IDC_RESTART_CHECK);
		hRestartValue = GetDlgItem(hDlg, IDC_RESTART_NB);
		hClipBoard = GetDlgItem(hDlg, IDC_CLIPBOARD_CHECK);
		hInputScreenRadio = GetDlgItem(hDlg, IDC_SCREEN_RADIO);
		hInputFilesRadio = GetDlgItem(hDlg, IDC_DIR_RADIO);
		hFilesDirectory = GetDlgItem(hDlg, IDC_DIRECTORY);
		hBrowseButton = GetDlgItem(hDlg, IDC_BROWSEDIRBUTTON);

		// Quick sanity check...
		if (!(hSpeed && hDelay && hRuleCombo && hVDIVCombo && hHDIVCombo && hPositionCombo 
			&& hPropButton && hRestartCheck && hRestartValue && hClipBoard && hInputScreenRadio
			&& hInputFilesRadio && hFilesDirectory && hBrowseButton))
		{
			// well if the controls don't exist it's probably some freaky low memory condition
			// and we (or the user) won't be able to do much anyway...

			// screen savers have no standard output or error, so we'll just have to hope somebody
			// has a debugger running...
			OutputDebugString("PrenzlSaver: (panic) Unable to get handles to dialog controls.\r\n");
			// yeah, CR/LF reminds me of the good ol' DOS days :-)

			return FALSE;		// hopefully will be able to shut down without crashing
		}

		// set up the slider
		SendMessage(hSpeed, TBM_SETRANGE, FALSE, MAKELONG(0, 250));
		SendMessage(hSpeed, TBM_SETPAGESIZE, 0, 5);

		if (!config.readFromRegistry())				// try to read configuration
			return FALSE;									// only proceed if we succeed

		char buffer[MAX_PATH];	

		long pos;
		pos = saturate(250 - config.delay, 0, 250);
		SendMessage(hSpeed, TBM_SETPOS, TRUE, pos);

		_itoa(config.delay, buffer, 10);			// yeah, yeah, damn unchecked buffer again
		SendMessage(hDelay, WM_SETTEXT, 0, (LPARAM)buffer);

		// init combo box
		for(unsigned int i=0; i< config.ruleFactories.size(); i++) {
			SendMessage(hRuleCombo, CB_ADDSTRING, 0, (LPARAM)config.ruleFactories[i]->getName().c_str());
		}

		// set combobox to the right item
		SendMessage(hRuleCombo, CB_SETCURSEL, config.ruleID, 0);

		// enable or disable the button
		EnableWindow(hPropButton, config.getRuleFactory().hasPropertyDialog());

		// init radio screen input / files input
		SendMessage(config.useFileAsInput ? hInputFilesRadio : hInputScreenRadio,
			BM_SETCHECK, TRUE, 0);

		// disable unused input components
		if(config.useFileAsInput) {
			EnableWindow(hVDIVCombo, false);
			EnableWindow(hHDIVCombo, false);
		}
		else {
			EnableWindow(hFilesDirectory, false);
			EnableWindow(hBrowseButton, false);
		}

		// init vertical divisions combo box
		SendMessage(hVDIVCombo, CB_ADDSTRING, 0, (LPARAM)"No Division");
		SendMessage(hVDIVCombo, CB_ADDSTRING, 0, (LPARAM)"Divide in 2");
		SendMessage(hVDIVCombo, CB_ADDSTRING, 0, (LPARAM)"Divide in 4");
		SendMessage(hVDIVCombo, CB_ADDSTRING, 0, (LPARAM)"Divide in 8");
		
		// set combobox to the right item
		SendMessage(hVDIVCombo, CB_SETCURSEL, config.vdiv, 0);

		// init horizontal divisions combo box
		SendMessage(hHDIVCombo, CB_ADDSTRING, 0, (LPARAM)"No Division");
		SendMessage(hHDIVCombo, CB_ADDSTRING, 0, (LPARAM)"Divide in 2");
		SendMessage(hHDIVCombo, CB_ADDSTRING, 0, (LPARAM)"Divide in 4");
		SendMessage(hHDIVCombo, CB_ADDSTRING, 0, (LPARAM)"Divide in 8");
		
		// set combobox to the right item
		SendMessage(hHDIVCombo, CB_SETCURSEL, config.hdiv, 0);


		// init horizontal divisions combo box
		SendMessage(hPositionCombo, CB_ADDSTRING, 0, (LPARAM)"Center");
		SendMessage(hPositionCombo, CB_ADDSTRING, 0, (LPARAM)"Tile");
		SendMessage(hPositionCombo, CB_ADDSTRING, 0, (LPARAM)"Random");
		SendMessage(hPositionCombo, CB_ADDSTRING, 0, (LPARAM)"Stretch");
		
		// set combobox to the right item
		SendMessage(hPositionCombo, CB_SETCURSEL, config.position, 0);

		// set directory value
		SendMessage(hFilesDirectory, WM_SETTEXT, 0, (LPARAM)config.inputDirectory.c_str());

		// restart checkBox and TextBox init
		SendMessage(hRestartCheck, BM_SETCHECK, config.restartActivated ? BST_CHECKED : BST_UNCHECKED, 0/*not used*/);

		_itoa(config.nbGenBeforeRestart, buffer, 10);			
		SendMessage(hRestartValue, WM_SETTEXT, 0, (LPARAM)buffer);

		EnableWindow(hRestartValue, config.restartActivated);

		SendMessage(hClipBoard, BM_SETCHECK, config.saveToClipBoard ? BST_CHECKED : BST_UNCHECKED, 0);

		return TRUE;			// everything's fine here

	case WM_HSCROLL:			// slider tracking -> update the delay text box
		pos = (long) SendMessage(hSpeed, TBM_GETPOS, 0, 0);
		pos = 250 - pos;		
		_itoa(pos, buffer, 10);
		SendMessage(hDelay, WM_SETTEXT, 0, (LPARAM)buffer);

		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case IDC_DELAY:		// delay value changed : update the slider
			{
				// get the value of the delay
				SendMessage(hDelay, WM_GETTEXT, 63, (LPARAM)buffer);
				int delay = atoi(buffer);

				// compute the slider position (with saturation)
				int position = saturate(250 - delay, 0, 250);

				// set the slider position
				SendMessage(hSpeed, TBM_SETPOS, TRUE, position);
			}
			break;

		case IDC_RULECOMBO: // if the rule is changed, see if we have properties to
							// enable or disable the properties button
			{
				// get the index of the selected rule in the combobox
				long ruleID = (long)SendMessage(hRuleCombo, CB_GETCURSEL, 0, 0);
				// enable or disable the button
				EnableWindow(hPropButton, config.ruleFactories[ruleID]->hasPropertyDialog());
			}
			break;

		case IDC_PROPBUTTON:	// if the Properties Button is pushed, open the properties dialog
			{	
				// get the index of the selected rule in the combobox
				long ruleID = (long)SendMessage(hRuleCombo, CB_GETCURSEL, 0, 0);
				config.ruleFactories[ruleID]->showProperties(hDlg);
			}
			break;

		case IDC_RESTART_CHECK: // enable or disable the restart Text Box
			{
				int checked = (int)SendMessage(hRestartCheck, BM_GETCHECK, 0, 0);
				EnableWindow(hRestartValue, checked);			
			}
			break;

		case IDC_SCREEN_RADIO: // enable or disable input file components
		case IDC_DIR_RADIO:
			{
				bool useFileAsInput = SendMessage(hInputFilesRadio, BM_GETCHECK, 0, 0) != 0;
				EnableWindow(hVDIVCombo, !useFileAsInput);
				EnableWindow(hHDIVCombo, !useFileAsInput);
				EnableWindow(hFilesDirectory, useFileAsInput);
				EnableWindow(hBrowseButton, useFileAsInput);
			}
			break;

		case IDC_BROWSEDIRBUTTON:
			{
				// open a openDialog
				BROWSEINFO bi = { 0 };
				bi.lpszTitle = _T("Select the Directory containing the Pictures");
				bi.hwndOwner = hDlg;
				LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
				if ( pidl != 0 ) {
					// get the name of the folder
					TCHAR path[MAX_PATH];
					if ( SHGetPathFromIDList ( pidl, path ) ) {
						SendMessage(hFilesDirectory, WM_SETTEXT, 0, (LPARAM)path);
					}

					// free memory used
					IMalloc * imalloc = 0;
					if ( SUCCEEDED( SHGetMalloc ( &imalloc )) ) {
						imalloc->Free ( pidl );
						imalloc->Release ( );
					}
				}
			}
			break;

		case IDOK:				// OK button pushed : save settings

			SendMessage(hDelay, WM_GETTEXT, 63, (LPARAM)buffer);
			config.delay = atoi(buffer);

			config.position = (long)SendMessage(hPositionCombo, CB_GETCURSEL, 0, 0);
			config.ruleID = (long)SendMessage(hRuleCombo, CB_GETCURSEL, 0, 0);
			config.vdiv = (long)SendMessage(hVDIVCombo, CB_GETCURSEL, 0, 0);
			config.hdiv = (long)SendMessage(hHDIVCombo, CB_GETCURSEL, 0, 0);

			config.useFileAsInput = SendMessage(hInputFilesRadio, BM_GETCHECK, 0, 0) != 0;

			SendMessage(hFilesDirectory, WM_GETTEXT, 255, (LPARAM)buffer);
			config.inputDirectory = std::string(buffer);

			config.restartActivated = SendMessage(hRestartCheck, BM_GETCHECK, 0, 0) != 0;

			SendMessage(hRestartValue, WM_GETTEXT, 63, (LPARAM)buffer);
			config.nbGenBeforeRestart = atoi(buffer);

			config.saveToClipBoard = (int)SendMessage(hClipBoard, BM_GETCHECK, 0, 0) != 0;
			
			config.writeToRegistry();

		case IDCANCEL:			// ending the dialog either way so just fall through
			EndDialog(hDlg, LOWORD(wParam) == IDOK);
			return TRUE;
		}
		break;
	}

	return FALSE;
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

