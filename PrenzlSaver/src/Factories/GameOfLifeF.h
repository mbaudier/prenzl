#pragma once
#include "RuleFactory.h"
#include "RegistryAccessor.h"
#include "Rules/stdrules/GameOfLife.h"

namespace Prenzl {

	class GameOfLifeF : public RuleFactory {
	public:
		Rule * createRule() {
			return new GameOfLife(); 
		}

		std::string getName() {
			return std::string("Game Of Life");
		}
	};

}