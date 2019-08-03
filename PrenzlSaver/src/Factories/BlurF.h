#pragma once
#include "RuleFactory.h"
#include "Rules/stdrules/Blur.h"
#include "PrenzlDialog.h"
#include "Model/Util.h"
#include "RegistryAccessor.h"

namespace Prenzl {

	class BlurF : public RuleFactory {

	public:

		BlurF()
			: disturbanceLevel(0)
		{
			readFromRegistry();		
		}

		Rule * createRule() {
			return new Blur((unsigned int)(12-disturbanceLevel)); 
		}

		std::string getName() {
			return std::string("Blur");
		}

		bool hasPropertyDialog() {
			return true;
		}

		void showProperties(HWND parent) {
			PropDialog dial(disturbanceLevel, parent);
			INT_PTR ret = dial.DoModal();

			if(ret == IDOK) {
				disturbanceLevel = dial.getDisturbanceLevel();
				writeToRegistry();
			}
		}

	private:
		//! Properties Dialog for Nicolas de Stael
		class PropDialog : public PrenzlDialog {

		public:

			PropDialog(int disturbanceLevel, HWND parent)
				: PrenzlDialog(IDD_BLURDIALOG, parent)
				, disturbanceLevel(disturbanceLevel)
			{}

			BOOL OnInitDialog( ) {
				addStringToComboBox(IDC_DISTURBANCELEVEL, "0 - Normal Blur");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "1");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "2");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "3");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "4 - Altered Colors");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "5");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "6");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "7 - Ink Stain");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "8");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "9 - Carnaval");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "10");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "11");
				addStringToComboBox(IDC_DISTURBANCELEVEL, "12 - Chaos");
				setComboBoxSelectedIndex(IDC_DISTURBANCELEVEL, disturbanceLevel);
//				initNumericComboBox(IDC_DISTURBANCELEVEL, 0, 12, disturbanceLevel);
				return TRUE;
			}

			void OnOK() {
				disturbanceLevel = getComboBoxSelectedIndex(IDC_DISTURBANCELEVEL);
				PrenzlDialog::OnOK();
			}

			int getDisturbanceLevel() {
				return disturbanceLevel;
			}

		private: 
			int disturbanceLevel;
		};

	private:

		void readFromRegistry() {
			RegistryAccessor accessor;
			disturbanceLevel = saturate(accessor.readDWORD("BLUR_DISTURBANCE", disturbanceLevel),0,12);
		}

		void writeToRegistry() {
			RegistryAccessor accessor;
			accessor.writeDWORD("BLUR_DISTURBANCE", disturbanceLevel);
		}

	private:
		int disturbanceLevel;

	};

}
