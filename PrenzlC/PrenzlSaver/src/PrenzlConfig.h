#pragma once
#include <vector>
#include <string>

#include "stdafx.h"
#include "RegistryAccessor.h"
#include "RuleFactory.h"

#include "Factories/OlNotBinF.h"
#include "Factories/BlurF.h"
#include "Factories/PhaseCheulouF.h"
#include "Factories/LsdF.h"
#include "Factories/NicolasDeStaelF.h"
#include "Factories/GameOfLifeF.h"
#include "Factories/ChannelPropagationF.h"
#include "Factories/TurnerF.h"
#include "Factories/HydraF.h"
#include "Factories/ColorHydraF.h"
#include "Factories/BraqueF.h"
#include "Factories/AnyRuleF.h"
#include "Factories/TestRuleF.h"

namespace Prenzl {

	class PrenzlConfig {
	public:

		enum POSITION {
			CENTER = 0,
			TILE = 1,
			RANDOM = 2,
			STRETCH = 3,
			MAX_VALUE = 3,
		};

		int position;
		int vdiv;
		int hdiv;
		int ruleID;
		int delay;				// milliseconds since windows timers aren't quite as high-resoltuion :-)
		int nbGenBeforeRestart;
		bool restartActivated;
		bool saveToClipBoard;
		bool useFileAsInput;
		std::string inputDirectory;
		int maxPictureRatio;		// max percentage of screen a picture can have
		bool recurseIntoSubDirectories;

		std::vector<RuleFactory*> ruleFactories;


		BOOL readFromRegistry() {
			// load the rules
			loadRules();

			try {
				RegistryAccessor accessor;
				delay = accessor.readDWORD("DELAY", 35);
				int randomRuleIndex = (int)ruleFactories.size() - 1; // also the last rule index
				ruleID = saturate(accessor.readDWORD("RULE_ID", randomRuleIndex), 0, randomRuleIndex); // Any Rule is default
				vdiv = saturate(accessor.readDWORD("V_DIV", 1), 0, 3); // split in 2 by default
				hdiv = saturate(accessor.readDWORD("H_DIV", 1), 0, 3); // split in 2 by default
				position = saturate(accessor.readDWORD("SCREEN_POSITION", RANDOM), 0, MAX_VALUE);
				nbGenBeforeRestart = saturate(accessor.readDWORD("RESTART_NB", 500), 0, 0x7FFFFFFF);
				restartActivated = accessor.readDWORD("RESTART_ACTIVATED", 1) != 0;
				saveToClipBoard = accessor.readDWORD("SAVE_TO_CLIPBOARD") != 0;
				useFileAsInput = accessor.readDWORD("FILE_AS_INPUT") != 0;
				inputDirectory = accessor.readString("INPUT_DIR");
				maxPictureRatio = saturate(accessor.readDWORD("MAX_PIC_RATIO", 50),10,100);
				recurseIntoSubDirectories = accessor.readDWORD("RECURSE_INTO_SUBDIR", 0) != 0;
				return TRUE;
			}
			catch(std::exception& /*e*/) {
				return FALSE;
			}
		}

		BOOL writeToRegistry() {
			try {
				RegistryAccessor accessor;
				accessor.writeDWORD("DELAY", delay);
				accessor.writeDWORD("RULE_ID", ruleID);
				accessor.writeDWORD("V_DIV", vdiv);
				accessor.writeDWORD("H_DIV", hdiv);
				accessor.writeDWORD("SCREEN_POSITION", position);
				accessor.writeDWORD("RESTART_NB", nbGenBeforeRestart);
				accessor.writeDWORD("RESTART_ACTIVATED", restartActivated ? 1 : 0);				
				accessor.writeDWORD("SAVE_TO_CLIPBOARD", saveToClipBoard ? 1 : 0);
				accessor.writeDWORD("FILE_AS_INPUT", useFileAsInput ? 1 : 0);
				accessor.writeString("INPUT_DIR", inputDirectory.c_str());
				accessor.writeDWORD("MAX_PIC_RATIO", maxPictureRatio);
				accessor.writeDWORD("RECURSE_INTO_SUBDIR", recurseIntoSubDirectories ? 1 : 0);
				return TRUE;
			}
			catch(std::exception& /*e*/) {
				return FALSE;
			}
		}

		void loadRules() {
			if(ruleFactories.size() == 0) {
				ruleFactories.push_back(new OlNotBinF());
				ruleFactories.push_back(new PhaseCheulouF());
				ruleFactories.push_back(new BlurF());
				ruleFactories.push_back(new LsdF());
				ruleFactories.push_back(new NicolasDeStaelF());
				ruleFactories.push_back(new GameOfLifeF());
				ruleFactories.push_back(new ChannelPropagationF());
				ruleFactories.push_back(new TurnerF());
				ruleFactories.push_back(new HydraF());
				ruleFactories.push_back(new ColorHydraF());
				ruleFactories.push_back(new BraqueF());
				ruleFactories.push_back(new AnyRuleF(ruleFactories));
				ruleFactories.push_back(new TestRuleF());
			}
		}

		Rule * createRule() {
			return ruleFactories[ruleID]->createRule();
		}

		RuleFactory& getRuleFactory() {
			return (*ruleFactories[ruleID]);
		}
	};				
}