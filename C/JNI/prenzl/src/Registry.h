#pragma once

#include <iostream.h>
#include <vector>

#include "Model/Rule.h"
#include "Model/Topology.h"

namespace Prenzl {

	class Registry {
	public:
		
		Registry() {
			cout << "# REGISTRY # Initialized" << std::endl;
		}

		~Registry() {	
			for(unsigned int i = 0; i < registerLines_.size(); i++) {
				registerLines_[i].invalidate();
			}
			cout << "# REGISTRY # Cleaned up" << std::endl;
		}

		int registerRuleAndTopology(Rule * rule,Topology* topology){
			
			// see if we have an invalid registerLine
			for(unsigned int i = 0; i < registerLines_.size(); i++) {
				if(	! registerLines_[i].isValid()) {
					// we found an invalid line, use it
					registerLines_[i].recycle(rule, topology);
					cout << "# REGISTRY (RECYCLE)# " << i <<"\t# Created new rule" << std::endl;
					return (int) i;
				}
			}
			
			// we did not find any invalid registerLine
			// just add a new line
			registerLines_.push_back(RegisterLine(rule, topology));
			cout << "# REGISTRY (ADD)# " << registerLines_.size() - 1 <<"\t# Created new rule" << std::endl;
			return (int) (registerLines_.size() - 1);
		}
		
		void remove(int index){
			registerLines_.at(index).invalidate();
			cout << "# REGISTRY # " << index <<"\t# Removed rule" << std::endl;
		}
		
		Rule* getRule(int index){
			return registerLines_.at(index).getRule();
		}
		Topology* getTopology(int index){
			return registerLines_.at(index).getTopology();
		}

	protected:
	
		class RegisterLine {
		public:
			RegisterLine(Rule * rule, Topology * topology)
			: valid_(true)
			, rule_(rule)
			, topology_(topology)
			{}
			
			void invalidate() {
				if(valid_) {
					valid_ = false;
					delete rule_;
					delete topology_;
					rule_ = 0;
					topology_ = 0;
				}
			}
			
			void recycle(Rule * rule, Topology * topology) {
				rule_ = rule;
				topology_ = topology;
				valid_ = true;
			}
			
			bool isValid() {
				return valid_;
			}
			
			Rule * getRule() {
				return rule_;
			}
			
			Topology * getTopology() {
				return topology_;
			}
			
		private:
			bool valid_;
			Rule * rule_;
			Topology * topology_;	
		};
	
		std::vector<RegisterLine> registerLines_;	
	};
}
