#pragma once
#include "Topology.h"


namespace Prenzl {

	class Rule {
	public:
		virtual ~Rule() {}
		virtual void computeNext(Topology& topo) = 0 ;


	};

}
