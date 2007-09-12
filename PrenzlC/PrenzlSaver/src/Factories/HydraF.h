#pragma once
#include "RuleFactory.h"
#include "Rules/stdrules/Hydra.h"

namespace Prenzl {

	class HydraF : public RuleFactory {

	public:

		HydraF() 
			: bodyLowerLimit(100)
			, headLowerLimit(200)
		{
			readFromRegistry();				
		}

		Rule * createRule() {
			return new Hydra(bodyLowerLimit, headLowerLimit); 
		}

		std::string getName() {
			return std::string("Hydra");
		}

		bool hasPropertyDialog() {
			return true;
		}

		void showProperties(HWND parent) {
			PropDialog dial(bodyLowerLimit, headLowerLimit, parent);
			INT_PTR ret = dial.DoModal();

			if(ret == IDOK) {
				dial.getValues(bodyLowerLimit, headLowerLimit);
				writeToRegistry();
			}
		}
	private:
		//! Properties Dialog for Nicolas de Stael
		class PropDialog : public PrenzlDialog {

		public:

			PropDialog(int bodyLowerLimit, int headLowerLimit, HWND parent)
				: PrenzlDialog(IDD_HYDRADIALOG, parent)
				, bodyLowerLimit(bodyLowerLimit)
				, headLowerLimit(headLowerLimit)
			{}

			BOOL OnInitDialog( ) {
				configureSlider(IDC_BODYLOWERSLIDER, 0, 255, 1);
				setSliderPosition(IDC_BODYLOWERSLIDER, bodyLowerLimit);

				configureSlider(IDC_HEADLOWERSLIDER, 0, 255, 1);
				setSliderPosition(IDC_HEADLOWERSLIDER, headLowerLimit);

				setTextBoxIntValue(IDC_BODYLOWEREDIT, bodyLowerLimit);
				setTextBoxIntValue(IDC_HEADLOWEREDIT, headLowerLimit);

				return TRUE;
			}

			void OnOK() {
				bodyLowerLimit = getSliderPosition(IDC_BODYLOWERSLIDER);
				headLowerLimit = getSliderPosition(IDC_HEADLOWERSLIDER);
				PrenzlDialog::OnOK();
			}

			BOOL OnCmdMsg(UINT nID, int nCode, void*pExtra, AFX_CMDHANDLERINFO*pHandlerInfo) {
				if(nID == IDC_BODYLOWERSLIDER) {
					int body = getSliderPosition(IDC_BODYLOWERSLIDER);
					int head = getSliderPosition(IDC_HEADLOWERSLIDER);
					setTextBoxIntValue(IDC_BODYLOWEREDIT, body);
					if(head < body) {
						setSliderPosition(IDC_HEADLOWERSLIDER, body);
					}
				}
				else if(nID == IDC_HEADLOWERSLIDER) {
					int body = getSliderPosition(IDC_BODYLOWERSLIDER);
					int head = getSliderPosition(IDC_HEADLOWERSLIDER);
					setTextBoxIntValue(IDC_HEADLOWEREDIT, head);
					if(head < body) {
						setSliderPosition(IDC_BODYLOWERSLIDER, head);
					}
				}
				return PrenzlDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
			}

			void getValues(int& body, int& head) {
				body = bodyLowerLimit;
				head = headLowerLimit;
			}

		private: 
			int bodyLowerLimit;
			int headLowerLimit;
		};

	private:
		void readFromRegistry() {
			RegistryAccessor accessor;
			bodyLowerLimit = saturate(accessor.readDWORD("HYDRA_LOWER_BODY", bodyLowerLimit),0,255);
			headLowerLimit = saturate(accessor.readDWORD("HYDRA_LOWER_HEAD", headLowerLimit),0,255);
		}

		void writeToRegistry() {
			RegistryAccessor accessor;
			accessor.writeDWORD("HYDRA_LOWER_BODY", bodyLowerLimit);
			accessor.writeDWORD("HYDRA_LOWER_HEAD", headLowerLimit);
		}

	private: 
		int bodyLowerLimit;
		int headLowerLimit;

	};

}
