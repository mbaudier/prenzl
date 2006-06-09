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
				this->SendDlgItemMessage(IDC_BODYLOWERSLIDER, TBM_SETRANGE, FALSE, MAKELONG(0, 255));
				this->SendDlgItemMessage(IDC_BODYLOWERSLIDER, TBM_SETPAGESIZE, 0, 1);
				this->SendDlgItemMessage(IDC_BODYLOWERSLIDER, TBM_SETPOS, TRUE, bodyLowerLimit);

				this->SendDlgItemMessage(IDC_HEADLOWERSLIDER, TBM_SETRANGE, FALSE, MAKELONG(0, 255));
				this->SendDlgItemMessage(IDC_HEADLOWERSLIDER, TBM_SETPAGESIZE, 0, 1);
				this->SendDlgItemMessage(IDC_HEADLOWERSLIDER, TBM_SETPOS, TRUE, headLowerLimit);

				setTextBoxIntValue(IDC_BODYLOWEREDIT, bodyLowerLimit);
				setTextBoxIntValue(IDC_HEADLOWEREDIT, headLowerLimit);

				return TRUE;
			}

			void OnOK() {
				bodyLowerLimit = (int) this->SendDlgItemMessage(IDC_BODYLOWERSLIDER, TBM_GETPOS, 0, 0);
				headLowerLimit = (int) this->SendDlgItemMessage(IDC_HEADLOWERSLIDER, TBM_GETPOS, 0, 0);
				PrenzlDialog::OnOK();
			}

			BOOL OnCmdMsg(UINT nID, int nCode, void*pExtra, AFX_CMDHANDLERINFO*pHandlerInfo) {
				if(nID == IDC_BODYLOWERSLIDER) {
					int body = (int) this->SendDlgItemMessage(IDC_BODYLOWERSLIDER, TBM_GETPOS, 0, 0);
					int head = (int) this->SendDlgItemMessage(IDC_HEADLOWERSLIDER, TBM_GETPOS, 0, 0);
					setTextBoxIntValue(IDC_BODYLOWEREDIT, body);
					if(head < body) {
						this->SendDlgItemMessage(IDC_HEADLOWERSLIDER, TBM_SETPOS, TRUE, body);
					}
				}
				else if(nID == IDC_HEADLOWERSLIDER) {
					int body = (int) this->SendDlgItemMessage(IDC_BODYLOWERSLIDER, TBM_GETPOS, 0, 0);
					int head = (int) this->SendDlgItemMessage(IDC_HEADLOWERSLIDER, TBM_GETPOS, 0, 0);
					setTextBoxIntValue(IDC_HEADLOWEREDIT, head);
					if(head < body) {
						this->SendDlgItemMessage(IDC_BODYLOWERSLIDER, TBM_SETPOS, TRUE, head);
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
