#pragma once
#include "RuleFactory.h"
#include "RegistryAccessor.h"
#include "Model/Util.h"
#include "PrenzlDialog.h"
#include "Rules/stdrules/Turner.h"


namespace Prenzl {

	class TurnerF : public RuleFactory {

	public:
		
		TurnerF()
			: volatility(2)
			, sign_bit(5)
			, dir_bit(6)
		{
			readFromRegistry();
		}

		Rule * createRule() {
			return new Turner(volatility,sign_bit,dir_bit); 
		}

		std::string getName() {
			return std::string("Turner");
		}

		bool hasPropertyDialog() {
			return true;
		}

		void showProperties(HWND parent) {
			PropDialog dial(volatility, sign_bit, dir_bit, parent);
			INT_PTR ret = dial.DoModal();

			if(ret == IDOK) {
				dial.getValues(volatility, sign_bit, dir_bit);
				writeToRegistry();
			}
		}
	private:

		//! Properties Dialog for Nicolas de Stael
		class PropDialog : public PrenzlDialog {
		public:
			PropDialog(int volatility, int sign_bit, int dir_bit, HWND parent)
				: PrenzlDialog(IDD_STAELDIALOG, parent)
				, volatility(volatility)
				, sign_bit(sign_bit)
				, dir_bit(dir_bit)
			{}

			BOOL OnInitDialog( ) {
				// we reuse the dialog of NicolasDeStael, so we have to
				// change the Title
				SetWindowText("Turner");
				initNumericComboBox(IDC_VOLATILITY_COMBO, 0, 7, volatility);
				initNumericComboBox(IDC_SIGNBIT_COMBO   , 0, 7, sign_bit);
				initNumericComboBox(IDC_DIRBIT_COMBO    , 0, 7, dir_bit);
				return TRUE;
			}

			void OnOK() {
				volatility = getComboBoxSelectedIndex(IDC_VOLATILITY_COMBO);
				sign_bit = getComboBoxSelectedIndex(IDC_SIGNBIT_COMBO);
				dir_bit = getComboBoxSelectedIndex(IDC_DIRBIT_COMBO);	
				PrenzlDialog::OnOK();
			}

			void getValues(int& vol, int& signb, int& dirb) {
				vol = volatility;
				signb = sign_bit;
				dirb = dir_bit;
			}

		private: 
			int volatility;
			int sign_bit;
			int dir_bit;

		};

	private:
		void readFromRegistry() {
			RegistryAccessor accessor;
			volatility = saturate(accessor.readDWORD("TURNER_VOLATILITY", volatility),0,7);
			sign_bit   = saturate(accessor.readDWORD("TURNER_SIGN_BIT", sign_bit),0,7);
			dir_bit    = saturate(accessor.readDWORD("TURNER_DIR_BIT", dir_bit),0,7);
		}

		void writeToRegistry() {
			RegistryAccessor accessor;
			accessor.writeDWORD("TURNER_VOLATILITY", volatility);
			accessor.writeDWORD("TURNER_SIGN_BIT", sign_bit);
			accessor.writeDWORD("TURNER_DIR_BIT", dir_bit);
		}

	private: 
		int volatility;
		int sign_bit;
		int dir_bit;

	};

}