#pragma once
#include "Model/Rule.h"


namespace Prenzl {


	class Pipes : public Rule {
	public:
		
		Pipes(int directionBit = 4, int unconnectedDecrease = 2, int connectedDecrease = 1,
			int creationDecrease = 1, int fullyConnectedDecrease=0) 
		{
			this->directionBit = directionBit;
			this->unconnectedDecrease = unconnectedDecrease;
			this->connectedDecrease = connectedDecrease;
			this->creationDecrease = creationDecrease;
			this->fullyConnectedDecrease = fullyConnectedDecrease;
		}

		inline unsigned int computeNew(unsigned int old, unsigned int first, unsigned int second) {
			bool firstPipe = ((first!=0) && (first & (1<<directionBit)));
			bool secondPipe = ((second!=0) && (second & (1<<directionBit)));
			
			if(firstPipe && secondPipe) {
				// pipes are connected in both directions
				return (old*2 + first + second + 2 - fullyConnectedDecrease) / 4 ;
			}				
			else if(firstPipe) {
				// only the first pipe is connected
				return 	(old + first+1- connectedDecrease) / 2 ;
			}
			else if(secondPipe) {
				// only the second pipe is connected
				return 	(old + second+1- connectedDecrease) / 2 ;
			}
			else {
				// no pipe connected
				// decrease if possible
				return (old >= unconnectedDecrease) ? (old - unconnectedDecrease) : 0;
			}
		}
		
		inline void collectNeighboursInfo(unsigned int value, unsigned int& sum, 
			unsigned int& nbAlive, bool mustBeVertical)
		{
			if(value) {
				if(mustBeVertical == ((value & (1<<directionBit))!=0)) {
					sum += value;
					nbAlive++;	
				}
			}
		}
		
		inline unsigned int computeColor(Topology& topo, int i, int j, int color) {
			unsigned int old = topo.getPrevious(i, j, color);
			
			if(old != 0) {
				// alive	
				if(old & (1<<directionBit) ) {
					// vertical	
					return computeNew(
							old,
							topo.getPrevious(i, j-1, color),
							topo.getPrevious(i, j+1, color)
						);
				}
				else {
					// horizonal
					return computeNew(
							old,
							topo.getPrevious(i-1, j, color),
							topo.getPrevious(i+1, j, color)
						);
				}
			}
			else {
				// dead	
				unsigned int sum = 0;
				unsigned int nbAlive = 0;
				// vertical 	
				collectNeighboursInfo(topo.getPrevious(i, j-1, color), sum, nbAlive, true);
				collectNeighboursInfo(topo.getPrevious(i, j+1, color), sum, nbAlive, true);					
				// horizontal	
				collectNeighboursInfo(topo.getPrevious(i-1, j, color), sum, nbAlive, false);
				collectNeighboursInfo(topo.getPrevious(i+1, j, color), sum, nbAlive, false);
				
				if(nbAlive == 1) {
					//create a pipe
					return (sum >= creationDecrease) ? (sum - creationDecrease) : 0;
				}
				return 0;
			}			
		}

		void computeNext(Topology& topo, int i, int j) {
			
			topo.setCurrent(i, j, Topology::BLUE, computeColor(topo, i, j, Topology::BLUE));
			topo.setCurrent(i, j, Topology::GREEN, computeColor(topo, i, j, Topology::GREEN));
			topo.setCurrent(i, j, Topology::RED, computeColor(topo, i, j, Topology::RED));
//			topo.setCurrent(i, j, Topology::GREEN, 0);
//			topo.setCurrent(i, j, Topology::RED, 0);
		}

		void computeNext(Topology& topo) {
			for(int i = 0; i < topo.getWidth(); i++) {
				for(int j = 0; j < topo.getHeight(); j++) {
					computeNext(topo, i, j);
				}
			}
		}

	private:
	
		int directionBit;	
		unsigned int unconnectedDecrease;
		unsigned int connectedDecrease;
		unsigned int fullyConnectedDecrease;
		unsigned int creationDecrease;
		
	};

}
