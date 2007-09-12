#pragma once
#include "RuleFactory.h"
#include "RegistryAccessor.h"
#include "Rules/stdrules/Lsd.h"
#include "PrenzlDialog.h"

namespace Prenzl {

	class LsdF : public RuleFactory {

	public:

		LsdF() 
			: deltaB_(1)
			, deltaG_(1)
			, deltaR_(2)
			, dxB_(-1)
			, dyB_(-1)
			, dxG_(1)
			, dyG_(0)
			, dxR_(0)
			, dyR_(1)
		{
			readFromRegistry();
		}

		Rule * createRule() {
			return new Lsd(deltaB_,deltaG_,deltaR_, dxB_, dyB_, dxG_, dyG_, dxR_, dyR_); 
		}

		std::string getName() {
			return std::string("LSD");
		}

		bool hasPropertyDialog() {
			return true;
		}

		void showProperties(HWND parent) {
			PropDialog dial(deltaB_, deltaG_, deltaR_, dxB_, dyB_, dxG_, dyG_, dxR_, dyR_, parent);
			INT_PTR ret = dial.DoModal();

			if(ret == IDOK) {
				dial.getDeltas(deltaB_, deltaG_, deltaR_);
				dial.getDxy(dxB_, dyB_, dxG_, dyG_, dxR_, dyR_);
				writeToRegistry();
			}
		}

	private:

		void readFromRegistry() {
			RegistryAccessor accessor;
			deltaB_ = (unsigned char)accessor.readDWORD("LSD_DELTA_B", deltaB_);
			deltaG_ = (unsigned char)accessor.readDWORD("LSD_DELTA_G", deltaG_);
			deltaR_ = (unsigned char)accessor.readDWORD("LSD_DELTA_R", deltaR_);
			dxB_    = (int)accessor.readDWORD("LSD_DX_B", dxB_);
			dyB_    = (int)accessor.readDWORD("LSD_DY_B", dyB_);
			dxG_    = (int)accessor.readDWORD("LSD_DX_G", dxG_);
			dyG_    = (int)accessor.readDWORD("LSD_DY_G", dyG_);
			dxR_    = (int)accessor.readDWORD("LSD_DX_R", dxR_);
			dyR_    = (int)accessor.readDWORD("LSD_DY_R", dyR_);
		}

		void writeToRegistry() {
			RegistryAccessor accessor;
			accessor.writeDWORD("LSD_DELTA_B", deltaB_);
			accessor.writeDWORD("LSD_DELTA_G", deltaG_);
			accessor.writeDWORD("LSD_DELTA_R", deltaR_);
			accessor.writeDWORD("LSD_DX_B", dxB_);
			accessor.writeDWORD("LSD_DY_B", dyB_);
			accessor.writeDWORD("LSD_DX_G", dxG_);
			accessor.writeDWORD("LSD_DY_G", dyG_);
			accessor.writeDWORD("LSD_DX_R", dxR_);
			accessor.writeDWORD("LSD_DY_R", dyR_);
		}

		//! Properties Dialog for LSD
		class PropDialog : public PrenzlDialog {
		public:
			PropDialog(unsigned char deltaB, unsigned char deltaG, unsigned char deltaR,
				int dxB, int dyB, int dxG, int dyG, int dxR, int dyR, HWND parent)
				: PrenzlDialog(IDD_LSDDIALOG, parent)
				, deltaB_(deltaB)
				, deltaG_(deltaG)
				, deltaR_(deltaR)
				, dxB_(dxB)
				, dyB_(dyB)
				, dxG_(dxG)
				, dyG_(dyG)
				, dxR_(dxR)
				, dyR_(dyR)
			{}

			BOOL OnInitDialog( ) {
				setTextBoxIntValue(IDC_DELTA_BLUE , deltaB_);
				setTextBoxIntValue(IDC_DELTA_GREEN, deltaG_);
				setTextBoxIntValue(IDC_DELTA_RED  , deltaR_);
				prepareComboBox(IDC_BLUE_DX , false, dxB_);
				prepareComboBox(IDC_BLUE_DY , true, dyB_);
				prepareComboBox(IDC_GREEN_DX, false, dxG_);
				prepareComboBox(IDC_GREEN_DY, true, dyG_);
				prepareComboBox(IDC_RED_DX  , false, dxR_);
				prepareComboBox(IDC_RED_DY  , true, dyR_);
				return TRUE;
			}

			void OnOK() {
				deltaB_ = getTextBoxIntValue(IDC_DELTA_BLUE);
				deltaG_ = getTextBoxIntValue(IDC_DELTA_GREEN);
				deltaR_ = getTextBoxIntValue(IDC_DELTA_RED);
				dxB_ = getDValue(IDC_BLUE_DX );
				dyB_ = getDValue(IDC_BLUE_DY );
				dxG_ = getDValue(IDC_GREEN_DX);
				dyG_ = getDValue(IDC_GREEN_DY);
				dxR_ = getDValue(IDC_RED_DX  );
				dyR_ = getDValue(IDC_RED_DY  );
				
				PrenzlDialog::OnOK();
			}

			void getDeltas(unsigned char& deltaB, unsigned char& deltaG, unsigned char& deltaR) {
				deltaB = deltaB_;
				deltaG = deltaG_;
				deltaR = deltaR_;
			}

			void getDxy(int& dxB, int& dyB, int& dxG, int& dyG, int& dxR, int& dyR) {
				dxB = dxB_;
				dyB = dyB_;
				dxG = dxG_;
				dyG = dyG_;
				dxR = dxR_;
				dyR = dyR_;	
			}

		private:

			void prepareComboBox(int nID, bool isDy, int dValue) {
				addStringToComboBox(nID, isDy ? "Up" : "Right");
				addStringToComboBox(nID, "None");
				addStringToComboBox(nID, isDy ? "Down" : "Left");
				setComboBoxSelectedIndex(nID, dValue + 1);
			}

			int getDValue(int nID) {
				return getComboBoxSelectedIndex(nID) - 1;
			}

		private:
			unsigned char deltaB_;
			unsigned char deltaG_;
			unsigned char deltaR_;
			int dxB_, dyB_, dxG_, dyG_, dxR_, dyR_;

		};


	private:
		unsigned char deltaB_;
		unsigned char deltaG_;
		unsigned char deltaR_;
		int dxB_, dyB_, dxG_, dyG_, dxR_, dyR_;

	};

}