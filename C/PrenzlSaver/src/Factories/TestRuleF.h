#pragma once
#include "RuleFactory.h"
#include "Model/Util.h"

namespace Prenzl {

	class TestRuleF : public RuleFactory {

	public:
		Rule * createRule() {
			return new TestRule(); 
		}

		std::string getName() {
			return std::string("TestRule");
		}

	private:
		class TestRule : public Rule 
		{

		public:

			TestRule()
			{}


			void computeNext(Topology& topo) {
				for(int i = 0; i < topo.getWidth(); i++) {
					for(int j = 0; j < topo.getHeight(); j++) {
						computeNext(topo, i, j);
					}
				}
			}

		private:
	
			static const int bodyLowerLimit = 100;
			static const int headLowerLimit = 200;

			static int getMax(int a, int b, int c) {
				return max(a, max(b, c));
			}

			void computeNext(Topology& topo, int i, int j) {

				// for blue
				int bC = topo.getPrevious(i + 0, j + 0, Topology::BLUE);

				// for green
				int gC = topo.getPrevious(i + 0, j + 0, Topology::GREEN);

				// for red
				int rC = topo.getPrevious(i + 0, j + 0, Topology::RED);

				// get the max and the max Color
				int maxC = getMax(bC, gC, rC);

				int newBlue = 0;
				int newGreen = 0;
				int newRed = 0;

				if(maxC >= headLowerLimit) {
					// we have a head
					// a head dies and becomes a body

					// we set the max to headLowerLimit - 1
					newBlue  = (bC >= headLowerLimit) ? (headLowerLimit - 1) : bC;
					newGreen = (gC >= headLowerLimit) ? (headLowerLimit - 1) : gC;
					newRed   = (rC >= headLowerLimit) ? (headLowerLimit - 1) : rC;
				}
				else if(maxC < bodyLowerLimit) {
					// we have a non animal
					// may become a head if 
					// - there is one and only one head around
					// - there is no body

					int bN = topo.getPrevious(i + 0, j - 1, Topology::BLUE);
					int bS = topo.getPrevious(i + 0, j + 1, Topology::BLUE);
					int bE = topo.getPrevious(i + 1, j + 0, Topology::BLUE);
					int bW = topo.getPrevious(i - 1, j + 0, Topology::BLUE);

					int gN = topo.getPrevious(i + 0, j - 1, Topology::GREEN);
					int gS = topo.getPrevious(i + 0, j + 1, Topology::GREEN);
					int gE = topo.getPrevious(i + 1, j + 0, Topology::GREEN);
					int gW = topo.getPrevious(i - 1, j + 0, Topology::GREEN);

					int rN = topo.getPrevious(i + 0, j - 1, Topology::RED);
					int rS = topo.getPrevious(i + 0, j + 1, Topology::RED);
					int rE = topo.getPrevious(i + 1, j + 0, Topology::RED);
					int rW = topo.getPrevious(i - 1, j + 0, Topology::RED);

					unsigned int nbHead = 0;
					unsigned int nbBody = 0;

					int maxN = getMax(bN, gN, rN);
					if(maxN >= headLowerLimit) {nbHead++;newBlue=bN;newGreen=gN;newRed=rN;}
					else if(maxN >= bodyLowerLimit) {nbBody++;}

					int maxS = getMax(bS, gS, rS);
					if(maxS >= headLowerLimit) {nbHead++;newBlue=bS;newGreen=gS;newRed=rS;}
					else if(maxS >= bodyLowerLimit) {nbBody++;}

					int maxE = getMax(bE, gE, rE);
					if(maxE >= headLowerLimit) {nbHead++;newBlue=bE;newGreen=gE;newRed=rE;}
					else if(maxE >= bodyLowerLimit) {nbBody++;}

					int maxW = getMax(bW, gW, rW);
					if(maxW >= headLowerLimit) {nbHead++;newBlue=bW;newGreen=gW;newRed=rW;}
					else if(maxW >= bodyLowerLimit) {nbBody++;}

					if((nbHead == 1)&&(nbBody==0)) {
						// become a head
						// let as it is
					}
					else {
						// blur with no correction (blur + a little bit darker)
						newBlue = (12*bC + bN + bS + bE + bW)/16;
						newGreen = (12*gC + gN + gS + gE + gW)/16;
						newRed = (12*rC + rN + rS + rE + rW)/16;
					}
				}
				else {
					// a body slowly returns to nature
					newBlue  = (bC >= bodyLowerLimit) ? (bC - 1) : bC;
					newGreen = (gC >= bodyLowerLimit) ? (gC - 1) : gC;
					newRed   = (rC >= bodyLowerLimit) ? (rC - 1) : rC;

//					newBlue = bC - 1;  if(newBlue < 0) {newBlue=0;}
//					newGreen = gC - 1; if(newGreen < 0) {newGreen=0;} 
//					newRed = rC - 1;   if(newRed < 0) {newRed=0;}
				}

				topo.setCurrent(i, j, Topology::BLUE,  newBlue);

				topo.setCurrent(i, j, Topology::GREEN, newGreen);

				topo.setCurrent(i, j, Topology::RED,   newRed);

			}
		};

	};

}