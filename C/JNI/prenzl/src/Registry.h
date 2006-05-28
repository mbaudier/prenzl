#pragma once

#include <iostream.h>

#include "Model/Rule.h"
#include "Model/Topology.h"

namespace Prenzl {

	class Registry {
	public:
		
		Registry() 
		{
			size_ = 64;
			rules_ = new Rule*[size_];
			topologies_ = new Topology*[size_];
			
			nextIndex_ = 0;
			cout << "# REGISTRY # Initialized\n";
			cout.flush();
		}

		~Registry() {
			for(int i=0;i<nextIndex_;i++){
				if(rules_[i]!=NULL){
					delete rules_[i];
				}
				if(topologies_[i]!=NULL){
					delete topologies_[i];
				}
			}
			delete[] rules_;
			delete[] topologies_;
			cout << "# REGISTRY # Cleaned up\n";
			cout.flush();
		}

		int registerRuleAndTopology(Rule * rule,Topology* topology){
			
			// Registers
			int id = nextIndex_;			
			rules_[id]=rule;
			topologies_[id]=topology;
			//FIXME: check size
			nextIndex_++;
			cout << "# REGISTRY # " << id <<"\t# Created new rule\n";
			cout.flush();
			return id;
		}
		
		void remove(int index){
			delete rules_[index];
			rules_[index]=NULL;
			delete topologies_[index];
			topologies_[index]=NULL;
			cout << "# REGISTRY # " << index <<"\t# Removed rule\n";
			cout.flush();
		}
		
		Rule* getRule(int index){
			return rules_[index];
		}
		Topology* getTopology(int index){
			return topologies_[index];
		}

	protected:
		Rule** rules_;
		Topology** topologies_;
		
	private:
		int nextIndex_;
		int size_;
	};
}
