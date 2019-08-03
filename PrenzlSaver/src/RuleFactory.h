#pragma once
#include <string>

#include "Model/Rule.h"


namespace Prenzl {

	class RuleFactory {

	public:
		virtual Rule * createRule() = 0;

		virtual std::string getName() = 0;

		virtual bool hasPropertyDialog() {
			return false;
		}

		virtual void showProperties(HWND parent) {

		}

		static bool alphaNameSort(RuleFactory * first, RuleFactory * second) {
			return first->getName() < second->getName();
		}

	};

}
