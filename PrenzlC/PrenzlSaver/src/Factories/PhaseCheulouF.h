#pragma once

#include <vector>
#include <utility> // for pair

#include "RuleFactory.h"
#include "PrenzlDialog.h"
#include "RegistryAccessor.h"
#include "ExhaustiveRandomGenerator.h"

#include "Rules/stdrules/PhaseCheulou.h"

namespace Prenzl {

	//! Phase Cheulou Rule Factory
	class PhaseCheulouF : public RuleFactory {

	public:

		PhaseCheulouF()
			: coeffs(0, 90, 0, 0, 0, 90, -90, 0, 0)
			, useRandomProfile(true)
		{
			readFromRegistry();
			createProfiles();

			randomGenerator = ExhaustiveRandomGenerator(profiles.size());
		}

		Rule * createRule() {
			if(useRandomProfile) {
				unsigned index = randomGenerator.next();
				if(index) {
					return createRule(profiles[index].second);
				}
				// index = 0 -> custom : use current parameters
			}

			return createRule(coeffs); 
		}

		std::string getName() {
			return std::string("Phase Cheulou");
		}

		bool hasPropertyDialog() {
			return true;
		}

		void showProperties(HWND parent) {
			PropDialog dial(coeffs, profiles, useRandomProfile, parent);
			INT_PTR ret = dial.DoModal();

			if(ret == IDOK) {
				coeffs = dial.coeffs;
				useRandomProfile = dial.useRandomProfile;
				writeToRegistry();
			}
		}
	private:

		//! All coefficients of Phase Cheulou
		class CoeffMatrix {
		public:
			CoeffMatrix(int coeffBB, int coeffBG, int coeffBR,
						int coeffGB, int coeffGG, int coeffGR,
						int coeffRB, int coeffRG, int coeffRR)
			    : coeffBG(coeffBG) , coeffBR(coeffBR) , coeffGR(coeffGR)
				, coeffGB(coeffGB) , coeffRB(coeffRB) , coeffRG(coeffRG)
				, coeffBB(coeffBB) , coeffGG(coeffGG) , coeffRR(coeffRR)
			{}

			bool equals(const CoeffMatrix& other) {
				return (  (coeffBB == other.coeffBB)
					    &&(coeffBG == other.coeffBG)
					    &&(coeffBR == other.coeffBR)
					    &&(coeffGB == other.coeffGB)
					    &&(coeffGG == other.coeffGG)
					    &&(coeffGR == other.coeffGR)
					    &&(coeffRB == other.coeffRB)
					    &&(coeffRG == other.coeffRG)
					    &&(coeffRR == other.coeffRR));
			}

			int coeffBB, coeffBG, coeffBR;
			int coeffGB, coeffGG, coeffGR;
			int coeffRB, coeffRG, coeffRR;
		};

		//! Properties Dialog for Phase Cheulou
		class PropDialog : public PrenzlDialog {
		public:
			PropDialog(	const CoeffMatrix& coeffs, 
						const std::vector< std::pair<std::string, CoeffMatrix> >& profiles, 
						bool useRandomProfile,
						HWND parent) 
				: PrenzlDialog(IDD_PHASECHEULOUDIALOG, parent)
				, coeffs(coeffs)
				, profiles(profiles)
				, useRandomProfile(useRandomProfile)
				, notificationSuspended(false)
			{}

			BOOL OnInitDialog( ) {
				// init the textbox
				applyCoeffMatrixToDisplay();

				// init the combobox
				initProfileCombobox();

				setCheckBoxChecked(IDC_RANDOMPROFILE, useRandomProfile);
				
				return TRUE;
			}

			void OnOK() {
				retrieveCoeffMatrixFromDisplay();
				useRandomProfile = checkBoxChecked(IDC_RANDOMPROFILE);
				PrenzlDialog::OnOK();
			}

			BOOL OnCmdMsg(UINT nID, int nCode, void*pExtra, AFX_CMDHANDLERINFO*pHandlerInfo) {
				// if notification is not suspended ...
				if(!notificationSuspended) {
					switch(nID) {
						case IDC_PHASECHEULOU_PROFIL:
							{
								int pos = getComboBoxSelectedIndex(IDC_PHASECHEULOU_PROFIL);
								// if pos == 0, do not do anything (custom)
								if((pos > 0)&&(pos < (long)profiles.size())) {
									coeffs = profiles[pos].second;
									applyCoeffMatrixToDisplay();
								}
							}
							break;
						case IDC_PHASECHEULOU_coeffBB:
						case IDC_PHASECHEULOU_coeffGB:
						case IDC_PHASECHEULOU_coeffRB:
						case IDC_PHASECHEULOU_coeffBG:
						case IDC_PHASECHEULOU_coeffGG:
						case IDC_PHASECHEULOU_coeffRG:
						case IDC_PHASECHEULOU_coeffBR:
						case IDC_PHASECHEULOU_coeffGR:
						case IDC_PHASECHEULOU_coeffRR:
							if(nCode == 768) { /*edit*/
								retrieveCoeffMatrixFromDisplay();
								updateProfileComboBoxSelection();
							}
							break;
					}
				}

				return PrenzlDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
			}

		private:

			void applyCoeffMatrixToDisplay() {
				notificationSuspended = true;
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffBG, coeffs.coeffBG);
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffBR, coeffs.coeffBR);
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffGR, coeffs.coeffGR);
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffGB, coeffs.coeffGB);
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffRB, coeffs.coeffRB);
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffRG, coeffs.coeffRG);
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffBB, coeffs.coeffBB);
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffGG, coeffs.coeffGG);
				setTextBoxIntValue(IDC_PHASECHEULOU_coeffRR, coeffs.coeffRR);
				notificationSuspended = false;
			}

			void retrieveCoeffMatrixFromDisplay() {
				coeffs.coeffBG = getTextBoxIntValue(IDC_PHASECHEULOU_coeffBG);
				coeffs.coeffBR = getTextBoxIntValue(IDC_PHASECHEULOU_coeffBR);
				coeffs.coeffGR = getTextBoxIntValue(IDC_PHASECHEULOU_coeffGR);
				coeffs.coeffGB = getTextBoxIntValue(IDC_PHASECHEULOU_coeffGB);
				coeffs.coeffRB = getTextBoxIntValue(IDC_PHASECHEULOU_coeffRB);
				coeffs.coeffRG = getTextBoxIntValue(IDC_PHASECHEULOU_coeffRG);
				coeffs.coeffBB = getTextBoxIntValue(IDC_PHASECHEULOU_coeffBB);
				coeffs.coeffGG = getTextBoxIntValue(IDC_PHASECHEULOU_coeffGG);
				coeffs.coeffRR = getTextBoxIntValue(IDC_PHASECHEULOU_coeffRR);
			}

			void initProfileCombobox() {
				// add each profile in the combobox, preserving order
				for(unsigned int i=0; i < profiles.size(); i++) {
					addStringToComboBox(IDC_PHASECHEULOU_PROFIL, profiles[i].first.c_str());
				}

				// set the selection
				updateProfileComboBoxSelection();
			}

			void updateProfileComboBoxSelection() {
				int indexToSet = 0;

				for(unsigned int i=1; i < profiles.size(); i++) {
					// see if the profile matches exactly the current coeffs
					if(coeffs.equals(profiles[i].second)) {
						indexToSet = i;
					}
				}

				// set the selection
				notificationSuspended = true;
				setComboBoxSelectedIndex(IDC_PHASECHEULOU_PROFIL, indexToSet);
				notificationSuspended = false;
			}

		public:
			CoeffMatrix coeffs;
			bool useRandomProfile;

		private:
			// profiles
			bool notificationSuspended;
			const std::vector< std::pair<std::string, CoeffMatrix> >& profiles;
		};

		void createProfiles() {
			// dummy : not used
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Custom", 
				CoeffMatrix(0, 0, 0, 0, 0, 0, 0, 0, 0)));

			// standard
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Standard", 
				CoeffMatrix(0, 90, 0, 0, 0, 90, -90, 0, 0)));

			// standard inverted
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Standard Inverted", 
				CoeffMatrix(0, 0, -90, 90, 0, 0, 0, 90, 0)));

			// standard GreenPink
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Standard GreenPink", 
				CoeffMatrix(0, 90, 0, 0, 0, -90, 90, 0, 0)));

			// standard Rainbow
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Standard Rainbow", 
				CoeffMatrix(0, -90, 0, 0, 0, 90, 90, 0, 0)));

			// Petrol
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Petrol", 
				CoeffMatrix(0, -90, 0, 0, 0, -90, -90, 0, 0)));

			// Marie
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Marie", 
				CoeffMatrix(0, 100, -100, -100, 0, 100, 100, -100, 0)));

			// Naive Sweden
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Naive Sweden", 
				CoeffMatrix(0, -90, -90, 90, 0, 90, 90, 90, 0)));

			// Naive
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Naive", 
				CoeffMatrix(0, -90, 90, 90, 0, -90, -90, -90, 0)));

			// Flashy Aztec
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Flashy Aztec", 
				CoeffMatrix(0, -200, 200, 90, 0, -90, -90, -90, 0)));

			// Obscenity
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Obscenity", 
				CoeffMatrix(0, -120, 120, 90 , 0, -90, -90, -90, 0)));

			// cristal
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Cristal", 
				CoeffMatrix(0, -200, -100, 100, -50, 100, 100, 100, 0)));

			// Nadja
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Nadja", 
				CoeffMatrix(0, -200, 200, 120 , 0, -120, -120, -120, 0)));

			// Psychedelic
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Psychedelic", 
				CoeffMatrix(0, -50, -100, 200, -50, 100, 100, -50, -20)));

			// Square
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Squares", 
				CoeffMatrix(90, 90, 90, -70, 0, 90, -70, 90, 0)));

			// Groussherzogtum
			profiles.push_back(std::pair<std::string, CoeffMatrix>("Groussherzogtum", 
				CoeffMatrix(0, 90, -100, 90, 0, -100, 90, 90, 90)));
		}

		static Rule * createRule(const CoeffMatrix& c) {
			return new PhaseCheulou(c.coeffBG, c.coeffBR, c.coeffGR, c.coeffGB, c.coeffRB,
									c.coeffRG, c.coeffBB, c.coeffGG, c.coeffRR);
		}

		void readFromRegistry() {
			RegistryAccessor accessor;
			coeffs.coeffBG = accessor.readDWORD("PhaseCheulou_coeffBG", coeffs.coeffBG);
			coeffs.coeffBR = accessor.readDWORD("PhaseCheulou_coeffBR", coeffs.coeffBR);
			coeffs.coeffGR = accessor.readDWORD("PhaseCheulou_coeffGR", coeffs.coeffGR);
			coeffs.coeffGB = accessor.readDWORD("PhaseCheulou_coeffGB", coeffs.coeffGB);
			coeffs.coeffRB = accessor.readDWORD("PhaseCheulou_coeffRB", coeffs.coeffRB);
			coeffs.coeffRG = accessor.readDWORD("PhaseCheulou_coeffRG", coeffs.coeffRG);
			coeffs.coeffBB = accessor.readDWORD("PhaseCheulou_coeffBB", coeffs.coeffBB);
			coeffs.coeffGG = accessor.readDWORD("PhaseCheulou_coeffGG", coeffs.coeffGG);
			coeffs.coeffRR = accessor.readDWORD("PhaseCheulou_coeffRR", coeffs.coeffRR);
			useRandomProfile = accessor.readDWORD("PhaseCheulou_randomProfile", useRandomProfile) != 0;
		}

		void writeToRegistry() {
			RegistryAccessor accessor;
			accessor.writeDWORD("PhaseCheulou_coeffBG", coeffs.coeffBG);
			accessor.writeDWORD("PhaseCheulou_coeffBR", coeffs.coeffBR);
			accessor.writeDWORD("PhaseCheulou_coeffGR", coeffs.coeffGR);
			accessor.writeDWORD("PhaseCheulou_coeffGB", coeffs.coeffGB);
			accessor.writeDWORD("PhaseCheulou_coeffRB", coeffs.coeffRB);
			accessor.writeDWORD("PhaseCheulou_coeffRG", coeffs.coeffRG);
			accessor.writeDWORD("PhaseCheulou_coeffBB", coeffs.coeffBB);
			accessor.writeDWORD("PhaseCheulou_coeffGG", coeffs.coeffGG);
			accessor.writeDWORD("PhaseCheulou_coeffRR", coeffs.coeffRR);
			accessor.writeDWORD("PhaseCheulou_randomProfile", useRandomProfile);
		}

	private:

		CoeffMatrix coeffs;
		bool useRandomProfile;
		std::vector< std::pair<std::string, CoeffMatrix> > profiles;

		ExhaustiveRandomGenerator randomGenerator;

	};

}
