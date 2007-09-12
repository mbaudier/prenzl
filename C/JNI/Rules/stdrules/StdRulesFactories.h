#pragma once
#include <map>
#include <string>

#include "Model/CProperties.h"
#include "Model/Rule.h"
namespace Prenzl {

class RFactory {
public:
	virtual std::string getName() = 0;
	virtual Rule * createRule(const CProperties& prop) = 0;
	virtual CProperties getDefaultProperties() {
		return CProperties();
	}
};

void registerFactory(RFactory * factory,
		std::map<std::string, RFactory *>& factories);

std::map<std::string, RFactory *> createFactories();
}
