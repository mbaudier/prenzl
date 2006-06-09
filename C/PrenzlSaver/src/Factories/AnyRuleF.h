#pragma once

#include <vector>

#include "RuleFactory.h"

namespace Prenzl {

	class AnyRuleF : public RuleFactory {
	public:
		
		AnyRuleF(const std::vector<RuleFactory*> ruleFactories)
			: ruleFactories(ruleFactories)
		{}

		Rule * createRule() {
			// get a random position in ruleFactories
			unsigned int randomIndex = rand() % (unsigned int)ruleFactories.size();
			return ruleFactories[randomIndex]->createRule(); 
		}

		std::string getName() {
			return std::string("Any (Random)");
		}
	private:

		std::vector<RuleFactory*> ruleFactories;

	};

}