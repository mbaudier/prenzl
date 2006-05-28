#pragma once
#include "RuleFactory.h"
#include "Rules/stdrules/OlNotBin.h"

namespace Prenzl {

	class OlNotBinF : public RuleFactory {

	public:
		Rule * createRule() {
			return new OlNotBin(); 
		}

		std::string getName() {
			return std::string("OlNotBin");
		}

	};

}
