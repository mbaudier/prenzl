#pragma once
#include "Model/Rule.h"

namespace Prenzl {


	class Genetic : public Rule {
	public:
		
		Genetic() 
		{
			adnDead.push_back(0);
			adnDead.push_back(0);
			adnDead.push_back(0);
			adnOld = adnDead;
			adnUp = adnDead;
			adnDown = adnDead;
			adnRight = adnDead;
			adnLeft = adnDead;
			adnNew = adnDead;
		}

		inline static void fillAdn(Topology& topo, int i, int j, std::vector<int>& adn) {
			adn[Topology::BLUE ] = topo.getPrevious(i, j, Topology::BLUE);
			adn[Topology::GREEN] = topo.getPrevious(i, j, Topology::GREEN);
			adn[Topology::RED  ] = topo.getPrevious(i, j, Topology::RED);
		}

		inline static void setAdn(Topology& topo, int i, int j, std::vector<int>& adn) {
			topo.setCurrent(i, j, Topology::BLUE, adn[Topology::BLUE ]);
			topo.setCurrent(i, j, Topology::GREEN, adn[Topology::GREEN ]);
			topo.setCurrent(i, j, Topology::RED, adn[Topology::RED ]);
		}
		
		inline static bool isAlive(std::vector<int>& adn) {
			return  (adn[Topology::BLUE] >= 128) 
				 || (adn[Topology::GREEN] >= 128) 
				 || (adn[Topology::RED] >= 128);	
		}

		inline static void updateStats(std::vector<int>& adn, int& nbCellAlives,
			std::vector<int>& adnTotal)
		{
			if(isAlive(adn)) {
				nbCellAlives++;
				adnTotal[Topology::BLUE] += adn[Topology::BLUE];
				adnTotal[Topology::GREEN] += adn[Topology::GREEN];
				adnTotal[Topology::RED] += adn[Topology::RED];
			}
		}

		inline static std::vector<int>& setDead(std::vector<int>& adn)
		{
			// get the max value
			int max = adn[Topology::BLUE];
			if(adn[Topology::GREEN]>max) {
				max = adn[Topology::GREEN];
			}
			if(adn[Topology::RED]>max) {
				max = adn[Topology::RED];
			}
			
			if(max > 127) {
				adn[Topology::BLUE] = (adn[Topology::BLUE]>(max-127)) ? adn[Topology::BLUE]-(max-127) : 0;
				adn[Topology::GREEN] = (adn[Topology::GREEN]>(max-127)) ? adn[Topology::GREEN]-(max-127) : 0;
				adn[Topology::RED] = (adn[Topology::RED]>(max-127)) ? adn[Topology::RED]-(max-127) : 0;
			}
			// else : do nothing
			
			return adn;
		}
		
		inline static std::vector<int>& decrease(std::vector<int>& adn, int step=1)
		{
			adn[Topology::BLUE] = (adn[Topology::BLUE]>step) ? adn[Topology::BLUE]-step : 0;
			adn[Topology::GREEN] = (adn[Topology::GREEN]>step) ? adn[Topology::GREEN]-step : 0;
			adn[Topology::RED] = (adn[Topology::RED]>step) ? adn[Topology::RED]-step : 0;
			return adn;
		}

		void computeNext(Topology& topo, int i, int j) {
		
			//init
			fillAdn(topo,i,j,adnOld);
			fillAdn(topo,i,j-1,adnUp);
			fillAdn(topo,i,j+1,adnDown);
			fillAdn(topo,i-1,j,adnRight);
			fillAdn(topo,i+1,j,adnLeft);

			int nbCellAlives = 0;
			adnNew = adnDead;
			
			updateStats(adnUp, nbCellAlives, adnNew);
			updateStats(adnDown, nbCellAlives, adnNew);
			updateStats(adnRight, nbCellAlives, adnNew);
			updateStats(adnLeft, nbCellAlives, adnNew);

						
			if(	isAlive(adnOld)	) {
				
				if((nbCellAlives==0)||(nbCellAlives==4)) {
					setAdn(topo,i,j,setDead(adnOld));
				}
				else {
					setAdn(topo,i,j,adnOld);
				}
			}
			else {
						
				if((nbCellAlives == 2)) {
					adnNew[Topology::BLUE] = (adnNew[Topology::BLUE] + 1) / 2;
					adnNew[Topology::GREEN] = (adnNew[Topology::GREEN] + 1) / 2;
					adnNew[Topology::RED] = (adnNew[Topology::RED] + 1) / 2;
					
					setAdn(topo,i,j,adnNew);
				}
				else if(nbCellAlives == 1) {
					setAdn(topo,i,j,adnNew);
				}
				else {
					setAdn(topo,i,j,decrease(adnOld));
				}
			}			
						
			
		}


		void computeNext(Topology& topo) {
			for(int i = 0; i < topo.getWidth(); i++) {
				for(int j = 0; j < topo.getHeight(); j++) {
					computeNext(topo, i, j);
				}
			}
		}

	private:
		std::vector<int> adnDead;
		std::vector<int> adnOld;
		std::vector<int> adnUp;
		std::vector<int> adnDown;
		std::vector<int> adnRight;
		std::vector<int> adnLeft;
		std::vector<int> adnNew;
		
		
	};

}
