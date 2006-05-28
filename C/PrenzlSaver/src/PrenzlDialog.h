#pragma once
#include "StdAfx.h"


namespace Prenzl {

	class PrenzlDialog : public CDialog {

	public:
		PrenzlDialog(UINT nID, HWND parent = 0) 
			: CDialog(nID)
			, parent_(parent)
		{}

		virtual INT_PTR DoModal( ) {
			::EnableWindow(parent_, false);
			return CDialog::DoModal();
		}

	protected:

		virtual void OnOK() {
			::EnableWindow(parent_, true);
			CDialog::OnOK();
		}

		virtual void OnCancel() {
			::EnableWindow(parent_, true);
			CDialog::OnCancel();
		}

		void setTextBoxIntValue(int nID, int value) {
			char buffer[64];	
			_itoa(value, buffer, 10);
			this->SendDlgItemMessage(nID, WM_SETTEXT, 0, (LPARAM)buffer);
		}

		int getTextBoxIntValue(int nID) {
			char buffer[64];	
			this->SendDlgItemMessage(nID, WM_GETTEXT, 63, (LPARAM)buffer);
			return atoi(buffer);
		}

		int getComboBoxSelectedIndex(int nID) {
			return (int)this->SendDlgItemMessage(nID, CB_GETCURSEL, 0, 0);
		}

		void setComboBoxSelectedIndex(int nID, int index) {
			this->SendDlgItemMessage(nID, CB_SETCURSEL, index, 0); 
		}

		void initNumericComboBox(int nID, int min, int max, int value) {
			// be sure value is between min and max
			// saturate if not
			value = saturate(value,min,max);

			char buffer[64];

			for(int i = min; i <= max; i++) {
				_itoa(i, buffer, 10);
				this->SendDlgItemMessage(nID, CB_ADDSTRING, 0, (LPARAM)buffer);
			}

			this->SendDlgItemMessage(nID, CB_SETCURSEL, value - min, 0); 
		}

		void addStringToComboBox(int nID, const char * value) {
			this->SendDlgItemMessage(nID, CB_ADDSTRING, 0, (LPARAM)value);
		}

	private:
		//! To have a real modal functioning
		HWND parent_;
	};

}