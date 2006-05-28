#pragma once
#include "RuleFactory.h"
#include "Rules/stdrules/Blur.h"

namespace Prenzl {

	class BlurF : public RuleFactory {

	public:
		Rule * createRule() {
			return new Blur(); 
		}

		std::string getName() {
			return std::string("Blur");
		}

	};

}
