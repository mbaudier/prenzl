#include "stdafx.h"	
#include "PrenzlConfDialog.h"

namespace Prenzl {

	bool PrenzlConfDialog::init(HWND hDlg) {
		// initialise all HWND
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
		hMaxPicRatio = GetDlgItem(hDlg, IDC_MAX_PIC_RATIO);
		hRecurseIntoSubDir = GetDlgItem(hDlg, IDC_RECURSE_INTO_SUBDIR);

		// Quick sanity check...
		if (!(hSpeed && hDelay && hRuleCombo && hVDIVCombo && hHDIVCombo && hPositionCombo 
			&& hPropButton && hRestartCheck && hRestartValue && hClipBoard && hInputScreenRadio
			&& hInputFilesRadio && hFilesDirectory && hBrowseButton && hMaxPicRatio 
			&& hRecurseIntoSubDir))
		{
			// well if the controls don't exist it's probably some freaky low memory condition
			// and we (or the user) won't be able to do much anyway...

			// screen savers have no standard output or error, so we'll just have to hope somebody
			// has a debugger running...
			OutputDebugString("PrenzlSaver: (panic) Unable to get handles to dialog controls.\r\n");
			// yeah, CR/LF reminds me of the good ol' DOS days :-)

			return FALSE;		// hopefully will be able to shut down without crashing
		}

		if (!config.readFromRegistry())						// try to read configuration
			return FALSE;									// only proceed if we succeed

		// set up the slider
		configureSlider(hSpeed, 0, 250, 5);
		int pos = saturate(250 - config.delay, 0, 250);
		setSliderPosition(hSpeed, (short)pos);

		setTextBoxIntValue(hDelay, config.delay);

		// init combo box
		for(unsigned int i=0; i< config.ruleFactories.size(); i++) {
			addStringToComboBox(hRuleCombo, config.ruleFactories[i]->getName());
		}

		// set combobox to the right item
		setComboBoxSelectedIndex(hRuleCombo, config.ruleID);

		// enable or disable the button
		EnableWindow(hPropButton, config.getRuleFactory().hasPropertyDialog());

		// init radio screen input / files input
		setCheckBoxChecked(config.useFileAsInput ? hInputFilesRadio : hInputScreenRadio, true);

		// disable unused input components
		updateDisplayForInput(config.useFileAsInput);

		// configure the division comboboxes
		configureDivisionComboBox(hVDIVCombo, config.vdiv);
		configureDivisionComboBox(hHDIVCombo, config.hdiv);

		// init horizontal divisions combo box
		addStringToComboBox(hPositionCombo, "Center");
		addStringToComboBox(hPositionCombo, "Tile");
		addStringToComboBox(hPositionCombo, "Random");
		addStringToComboBox(hPositionCombo, "Stretch");
		
		// set combobox to the right item
		setComboBoxSelectedIndex(hPositionCombo, config.position);

		// set directory value
		setTextBoxStringValue(hFilesDirectory, config.inputDirectory);

		// restart checkBox and TextBox init
		setCheckBoxChecked(hRestartCheck, config.restartActivated);

		setTextBoxIntValue(hRestartValue, config.nbGenBeforeRestart);

		EnableWindow(hRestartValue, config.restartActivated);

		setCheckBoxChecked(hClipBoard, config.saveToClipBoard);

		setTextBoxIntValue(hMaxPicRatio, config.maxPictureRatio);

		setCheckBoxChecked(hRecurseIntoSubDir, config.recurseIntoSubDirectories);

		return TRUE;			// everything's fine here
	}

	void PrenzlConfDialog::configureDivisionComboBox(HWND hCombo, int selectedIndex) {
		addStringToComboBox(hCombo, "No Split");
		addStringToComboBox(hCombo, "Split in 2");
		addStringToComboBox(hCombo, "Split in 4");
		addStringToComboBox(hCombo, "Split in 8");
		
		// set combobox to the right item
		setComboBoxSelectedIndex(hCombo, selectedIndex);
	}

	bool PrenzlConfDialog::updateDelay() {
		int delay = 250 - getSliderPosition(hSpeed);
		setTextBoxIntValue(hDelay, delay);
		return TRUE;
	}

	void PrenzlConfDialog::updateSpeed() {
		// get the value of the delay
		int delay = getTextBoxIntValue(hDelay);

		// compute the slider position (with saturation)
		int position = saturate(250 - delay, 0, 250);

		// set the slider position
		setSliderPosition(hSpeed, (short)position);
	}

	void PrenzlConfDialog::writeToRegistry() {
		config.delay = getTextBoxIntValue(hDelay);
		config.position = getComboBoxSelectedIndex(hPositionCombo);
		config.ruleID = getComboBoxSelectedIndex(hRuleCombo);
		config.vdiv = getComboBoxSelectedIndex(hVDIVCombo);
		config.hdiv = getComboBoxSelectedIndex(hHDIVCombo);
		config.useFileAsInput = checkBoxChecked(hInputFilesRadio);
		config.inputDirectory = getTextBoxStringValue(hFilesDirectory);
		config.restartActivated = checkBoxChecked(hRestartCheck);
		config.nbGenBeforeRestart = getTextBoxIntValue(hRestartValue);
		config.saveToClipBoard = checkBoxChecked(hClipBoard);				
		config.maxPictureRatio = getTextBoxIntValue(hMaxPicRatio);
		config.recurseIntoSubDirectories = checkBoxChecked(hRecurseIntoSubDir);

		// save the config in the registry
		config.writeToRegistry();
	}

	void PrenzlConfDialog::selectPictureDirectory(HWND hDlg) {
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

	bool PrenzlConfDialog::processCommand(HWND hDlg, WPARAM wParam) {
		switch(LOWORD(wParam)) {

		case IDC_DELAY:		// delay value changed : update the slider
			updateSpeed();
			break;

		case IDC_RULECOMBO: // if the rule is changed, see if we have properties to
							// enable or disable the properties button
			{
				// get the index of the selected rule in the combobox
				long ruleID = getComboBoxSelectedIndex(hRuleCombo);
				// enable or disable the button
				EnableWindow(hPropButton, config.ruleFactories[ruleID]->hasPropertyDialog());
			}
			break;

		case IDC_PROPBUTTON:	// if the Properties Button is pushed, open the properties dialog
			{	
				// get the index of the selected rule in the combobox
				long ruleID = getComboBoxSelectedIndex(hRuleCombo);
				config.ruleFactories[ruleID]->showProperties(hDlg);
			}
			break;

		case IDC_RESTART_CHECK: // enable or disable the restart Text Box
			{
				bool restart = checkBoxChecked(hRestartCheck);
				EnableWindow(hRestartValue, restart);			
			}
			break;

		case IDC_SCREEN_RADIO: // enable or disable input file components
		case IDC_DIR_RADIO:
			{
				bool useFileAsInput = checkBoxChecked(hInputFilesRadio);
				updateDisplayForInput(useFileAsInput);
			}
			break;

		case IDC_BROWSEDIRBUTTON:
			selectPictureDirectory(hDlg);
			break;

		case IDOK:				// OK button pushed : save settings
			writeToRegistry();
		case IDCANCEL:			// ending the dialog either way so just fall through
			EndDialog(hDlg, LOWORD(wParam) == IDOK);
			return TRUE;
		}
		return FALSE;
	}

	bool PrenzlConfDialog::processMessage(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

		switch(message) {
		case WM_INITDIALOG:
			return init(hDlg);
		case WM_HSCROLL:
			return updateDelay();
		case WM_COMMAND:
			return processCommand(hDlg, wParam);
		}
		return FALSE;
	}

	void PrenzlConfDialog::updateDisplayForInput(bool useFileAsInput) {
		EnableWindow(hVDIVCombo, !useFileAsInput);
		EnableWindow(hHDIVCombo, !useFileAsInput);
		EnableWindow(hFilesDirectory, useFileAsInput);
		EnableWindow(hBrowseButton, useFileAsInput);
		EnableWindow(hMaxPicRatio, useFileAsInput);
		EnableWindow(hRecurseIntoSubDir, useFileAsInput);
	}

	void PrenzlConfDialog::setTextBoxIntValue(HWND hwnd, int value) {
		char buffer[64];	
		_itoa(value, buffer, 10);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)buffer);
	}

	int PrenzlConfDialog::getTextBoxIntValue(HWND hwnd) {
		char buffer[64];	
		SendMessage(hwnd, WM_GETTEXT, 63, (LPARAM)buffer);
		return atoi(buffer);
	}

	void PrenzlConfDialog::setTextBoxStringValue(HWND hwnd, const std::string& value) {
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)value.c_str());
	}

	std::string PrenzlConfDialog::getTextBoxStringValue(HWND hwnd) {
		char buffer[MAX_PATH];	
		SendMessage(hwnd, WM_GETTEXT, MAX_PATH - 1, (LPARAM)buffer);
		return std::string(buffer);
	}

	bool PrenzlConfDialog::checkBoxChecked(HWND hwnd) {
		return SendMessage(hwnd, BM_GETCHECK, 0, 0) != 0;
	}

	void PrenzlConfDialog::setCheckBoxChecked(HWND hwnd, bool checked) {
		SendMessage(hwnd, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0/*not used*/);
	}

	int PrenzlConfDialog::getComboBoxSelectedIndex(HWND hwnd) {
		return (int)SendMessage(hwnd, CB_GETCURSEL, 0, 0);
	}

	void PrenzlConfDialog::setComboBoxSelectedIndex(HWND hwnd, int index) {
		SendMessage(hwnd, CB_SETCURSEL, index, 0); 
	}

	void PrenzlConfDialog::addStringToComboBox(HWND hwnd, const std::string& value) {
		SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)value.c_str());
	}

	void PrenzlConfDialog::configureSlider(HWND hwnd, unsigned short min, unsigned short max, unsigned short step) {
		SendMessage(hwnd, TBM_SETRANGE, FALSE, MAKELONG(min, max));
		SendMessage(hwnd, TBM_SETPAGESIZE, 0, step);
	}

	void PrenzlConfDialog::setSliderPosition(HWND hwnd, unsigned short position) {
		SendMessage(hwnd, TBM_SETPOS, TRUE, position);
	}

	int PrenzlConfDialog::getSliderPosition(HWND hwnd) {
		return (int)SendMessage(hwnd, TBM_GETPOS, 0, 0);
	}

}