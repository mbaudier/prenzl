#pragma once

#include "PrenzlConfig.h"

namespace Prenzl {

	class PrenzlConfDialog {

	public:

		bool init(HWND hDlg);

		void configureDivisionComboBox(HWND hCombo, int selectedIndex);

		bool updateDelay();

		void updateSpeed();

		void writeToRegistry();

		void selectPictureDirectory(HWND hDlg);

		bool processCommand(HWND hDlg, WPARAM wParam);

		bool processMessage(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

		void updateDisplayForInput(bool useFileAsInput);

		void setTextBoxIntValue(HWND hwnd, int value);

		int getTextBoxIntValue(HWND hwnd);

		void setTextBoxStringValue(HWND hwnd, const std::string& value);

		std::string getTextBoxStringValue(HWND hwnd);

		bool checkBoxChecked(HWND hwnd);

		void setCheckBoxChecked(HWND hwnd, bool checked);

		int getComboBoxSelectedIndex(HWND hwnd);

		void setComboBoxSelectedIndex(HWND hwnd, int index);

		void addStringToComboBox(HWND hwnd, const std::string& value);

		void configureSlider(HWND hwnd, unsigned short min, unsigned short max, unsigned short step);

		void setSliderPosition(HWND hwnd, unsigned short position);

		int getSliderPosition(HWND hwnd);

	private:

		HWND hSpeed;
		HWND hDelay;
		HWND hRuleCombo;
		HWND hVDIVCombo;
		HWND hHDIVCombo;
		HWND hPositionCombo;
		HWND hPropButton;
		HWND hRestartCheck;
		HWND hRestartValue;
		HWND hClipBoard;
		HWND hInputScreenRadio;
		HWND hInputFilesRadio;
		HWND hFilesDirectory;
		HWND hBrowseButton;
		HWND hMaxPicRatio;
		HWND hRecurseIntoSubDir;

		PrenzlConfig config;
	};

}