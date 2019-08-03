#pragma once
#include "RuleFactory.h"
#include "Rules/stdrules/Braque.h"

namespace Prenzl {

	class BraqueF : public RuleFactory {

	public:

		BraqueF()
			: volatility(2)
			, shift(2)
		{
			readFromRegistry();
		}

		Rule * createRule() {
			return new Braque(volatility, shift); 
		}

		std::string getName() {
			return std::string("Braque");
		}

		bool hasPropertyDialog() {
			return true;
		}

		void showProperties(HWND parent) {
			PropDialog dial(volatility, shift, parent);
			INT_PTR ret = dial.DoModal();

			if(ret == IDOK) {
				dial.getValues(volatility, shift);
				writeToRegistry();
			}
		}
	private:

		//! Properties Dialog for Nicolas de Stael
		class PropDialog : public PrenzlDialog {
		public:
			PropDialog(int volatility, int shift, HWND parent)
				: PrenzlDialog(IDD_BRAQUEDIALOG, parent)
				, volatility(volatility)
				, shift(shift)
			{}

			BOOL OnInitDialog( ) {
				initNumericComboBox(IDC_VOLATILITY_COMBO, 0, 7, volatility);
				initNumericComboBox(IDC_SIGNBIT_COMBO   , 0, 5, shift);
				return TRUE;
			}

			void OnOK() {
				volatility = getComboBoxSelectedIndex(IDC_VOLATILITY_COMBO);
				shift = getComboBoxSelectedIndex(IDC_SIGNBIT_COMBO);
				PrenzlDialog::OnOK();
			}

			void getValues(int& vol, int& sh) {
				vol = volatility;
				sh = shift;
			}

		private: 
			int volatility;
			int shift;

		};

		void readFromRegistry() {
			RegistryAccessor accessor;
			volatility = saturate(accessor.readDWORD("BRAQUE_VOLATILITY", volatility),0,7);
			shift   = saturate(accessor.readDWORD("BRAQUE_SIGN_BIT", shift),0,5);
		}

		void writeToRegistry() {
			RegistryAccessor accessor;
			accessor.writeDWORD("BRAQUE_VOLATILITY", volatility);
			accessor.writeDWORD("BRAQUE_SIGN_BIT", shift);
		}

	private:
		int volatility;
		int shift;


	};

}
