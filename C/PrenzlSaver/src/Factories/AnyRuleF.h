#pragma once

#include <vector>
#include <algorithm>

#include "RuleFactory.h"

namespace Prenzl {

	class AnyRuleF : public RuleFactory {
	public:
		
		AnyRuleF(const std::vector<RuleFactory*>& ruleFactories)
			: allRuleFactories_(ruleFactories)
		{
			readFromRegistry();		
		}

		Rule * createRule() {
			// get a random position in ruleFactories
			unsigned int randomIndex = rand() % (unsigned int)activatedRuleFactories_.size();
			return activatedRuleFactories_[randomIndex]->createRule(); 
		}

		std::string getName() {
			return std::string("Any (Random)");
		}

		bool hasPropertyDialog() {
			return true;
		}

		void showProperties(HWND parent) {
			PropDialog dial(allRuleFactories_, activatedRuleFactories_, parent);
			INT_PTR ret = dial.DoModal();

			if(ret == IDOK) {
				activatedRuleFactories_ = dial.activatedRuleFactories_;
				writeToRegistry();
			}
		}
	private:

		//! Properties Dialog for Nicolas de Stael
		class PropDialog : public PrenzlDialog {
		public:
			PropDialog(	const std::vector<RuleFactory*>& allRuleFactories,
						std::vector<RuleFactory*>& activatedRuleFactories,
						HWND parent)
				: PrenzlDialog(IDD_ANYRULEDIALOG, parent)
				, activatedRuleFactories_(activatedRuleFactories)
			{
				// fill the nonActivatedRuleFactories_
				for(unsigned int i=0; i < allRuleFactories.size(); i++) {
					if(!isActivated(allRuleFactories[i])) {
						nonActivatedRuleFactories_.push_back(allRuleFactories[i]);
					}
				}
			}

			BOOL OnInitDialog( ) {
				updateListBoxes();
				return TRUE;
			}

			void OnOK() {
				PrenzlDialog::OnOK();
			}

			BOOL OnCmdMsg(UINT nID, int nCode, void*pExtra, AFX_CMDHANDLERINFO*pHandlerInfo) {
				switch(nID) {
					case IDC_ADDBUTTON:
						{
							//get the selected index in the Non Activated list
							unsigned int selectionIndex;
							if(listBoxHasSelection(IDC_NONACTIVATEDRULES, selectionIndex)) {
								activate(nonActivatedRuleFactories_[selectionIndex]);
								// update display
								updateListBoxes();
							}

						}
						break;
					case IDC_REMOVEBUTTON:
						{
							//get the selected index in the Activated list
							unsigned int selectionIndex;
							if(listBoxHasSelection(IDC_ACTIVATEDRULES, selectionIndex)) {
								desactivate(activatedRuleFactories_[selectionIndex]);
								// update display
								updateListBoxes();
							}
						}
						break;
				}

				return PrenzlDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
			}

		private:
	
			bool isActivated(const RuleFactory* factory) {
				return std::find(activatedRuleFactories_.begin(), activatedRuleFactories_.end(), factory)
						!= activatedRuleFactories_.end();
			}

			void transfer(std::vector<RuleFactory*>& from, std::vector<RuleFactory*>& to, RuleFactory* factory) {
				// find the factory in the from vector
				std::vector<RuleFactory*>::iterator naItr = std::find(from.begin(),from.end(), factory);
				if(naItr != from.end()) {
					// if we found it, erase and add
					from.erase(naItr);
					to.push_back(factory);
				}			
			}

			void activate(RuleFactory* factory) {
				transfer(nonActivatedRuleFactories_, activatedRuleFactories_, factory);
			}

			void desactivate(RuleFactory* factory) {
				transfer(activatedRuleFactories_, nonActivatedRuleFactories_, factory);
			}

			void updateListBoxes() {
				// sort the rules lists
				std::sort(activatedRuleFactories_.begin(), activatedRuleFactories_.end(), RuleFactory::alphaNameSort);
				std::sort(nonActivatedRuleFactories_.begin(), nonActivatedRuleFactories_.end(), RuleFactory::alphaNameSort);

				// clear list Box and fill it
				clearListBox(IDC_ACTIVATEDRULES);
				for(unsigned int i = 0; i < activatedRuleFactories_.size(); i++) {
					addStringToListBox(IDC_ACTIVATEDRULES, activatedRuleFactories_.at(i)->getName().c_str());
				}
				clearListBox(IDC_NONACTIVATEDRULES);
				for(unsigned int i = 0; i < nonActivatedRuleFactories_.size(); i++) {
					addStringToListBox(IDC_NONACTIVATEDRULES, nonActivatedRuleFactories_.at(i)->getName().c_str());
				}
			}

		public: 
			std::vector<RuleFactory*> activatedRuleFactories_;
			std::vector<RuleFactory*> nonActivatedRuleFactories_;

		};

	private:

		void activateRuleOfName(const std::string& ruleName) {
			std::vector<RuleFactory*>::const_iterator itr = allRuleFactories_.begin();
			for(; itr != allRuleFactories_.end(); ++itr) {
				if((*itr)->getName() == ruleName) {
					activatedRuleFactories_.push_back(*itr);
					return;
				}
			}
			// not found
			// we do nothing
		}

		void readFromRegistry() {
			RegistryAccessor accessor;
			// read the activated Rules registry string
			std::string activatedReg = accessor.readString("ANYRULE_ACTIVATED");

			// parse the string
			// all rules are separated with a ';'
			
			while(true) {
				std::string::size_type pos = activatedReg.find(';',0);
				if(pos == std::string::npos) {
					// not found
					// it is the last string
					activateRuleOfName(activatedReg);
					break;
				}
				else {
					// found
					std::string ruleName = activatedReg.substr(0, pos);
					activateRuleOfName(ruleName);
					activatedReg = activatedReg.substr(pos + 1);
				}
			}

			// if activatedRuleFactories_ is empty, activate all the rules
			if(activatedRuleFactories_.size() == 0) {
				activatedRuleFactories_ = allRuleFactories_;
			}
		}

		void writeToRegistry() {
			RegistryAccessor accessor;
			std::string activatedReg;
			if(activatedRuleFactories_.size() == 0) {
				activatedReg = "";
			}
			else {
				// write the first rule
				activatedReg = activatedRuleFactories_.at(0)->getName();
				for(unsigned int i = 1; i < activatedRuleFactories_.size(); i++) {
					activatedReg += ";" ;
					activatedReg += activatedRuleFactories_.at(i)->getName();
				}
			}
			accessor.writeString("ANYRULE_ACTIVATED", activatedReg.c_str());
		}


	private:

		std::vector<RuleFactory*> activatedRuleFactories_;
		std::vector<RuleFactory*> allRuleFactories_;

	};

}