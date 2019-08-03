#pragma once
#include "RuleFactory.h"
#include "RegistryAccessor.h"
#include "PrenzlDialog.h"
#include "Rules/stdrules/ChannelPropagation.h"
#include "Model/Util.h"

namespace Prenzl {

	class ChannelPropagationF : public RuleFactory {

	public:
		
		ChannelPropagationF()
			: cB(500)
			, cG(500)
			, cR(500)
			, colorInteraction(0)
		{
			readFromRegistry();
		}

		Rule * createRule() {
			return new ChannelPropagation(cB,cG,cR,colorInteraction); 
		}

		std::string getName() {
			return std::string("Channel Propagation");
		}

		bool hasPropertyDialog() {
			return true;
		}

		void showProperties(HWND parent) {
			PropDialog dial(cB,cG,cR,colorInteraction, parent);
			INT_PTR ret = dial.DoModal();

			if(ret == IDOK) {
				dial.getValues(cB,cG,cR,colorInteraction);
				writeToRegistry();
			}
		}
	private:

		//! Properties Dialog for Nicolas de Stael
		class PropDialog : public PrenzlDialog {
		public:
			PropDialog(int cB, int cG, int cR, int colorInteraction, HWND parent)
				: PrenzlDialog(IDD_CHANNELPROPDIALOG, parent)
				, cB(cB)
				, cG(cG)
				, cR(cR)
				, colorInteraction(colorInteraction)
			{}

			BOOL OnInitDialog( ) {
				setTextBoxIntValue(IDC_COEFF_B, cB);
				setTextBoxIntValue(IDC_COEFF_G, cG);
				setTextBoxIntValue(IDC_COEFF_R, cR);
				setTextBoxIntValue(IDC_COLOR_INTERACTION, colorInteraction);
				return TRUE;
			}

			void OnOK() {
				cB = getTextBoxIntValue(IDC_COEFF_B);
				cG = getTextBoxIntValue(IDC_COEFF_G);
				cR = getTextBoxIntValue(IDC_COEFF_R);
				colorInteraction = getTextBoxIntValue(IDC_COLOR_INTERACTION);
				PrenzlDialog::OnOK();
			}

			void getValues(int& c_B, int& c_G, int& c_R, int& colorInter) {
				c_B = cB;
				c_G = cG;
				c_R = cR;
				colorInter = colorInteraction;
			}

		private: 
			int cB, cG, cR;
			int colorInteraction;

		};

	private:
		void readFromRegistry() {
			RegistryAccessor accessor;
			cB = accessor.readDWORD("CHANNEL_PROP_CB", cB);
			cG = accessor.readDWORD("CHANNEL_PROP_CG", cG);
			cR = accessor.readDWORD("CHANNEL_PROP_CR", cR);
			colorInteraction    = accessor.readDWORD("CHANNEL_PROP_COLOR_INTERACTION", colorInteraction);
		}

		void writeToRegistry() {
			RegistryAccessor accessor;
			accessor.writeDWORD("CHANNEL_PROP_CB", cB);
			accessor.writeDWORD("CHANNEL_PROP_CG", cG);
			accessor.writeDWORD("CHANNEL_PROP_CR", cR);
			accessor.writeDWORD("CHANNEL_PROP_COLOR_INTERACTION", colorInteraction);
		}

	private: 
		int cB, cG, cR;
		int colorInteraction;

	};

}