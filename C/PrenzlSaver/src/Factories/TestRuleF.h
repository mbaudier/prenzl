#pragma once
#include "RuleFactory.h"

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
		class TestRule : public Rule {

		public:
			void computeNext(Topology& topo) {
				for(int i = 0; i < topo.getWidth(); i++) {
					for(int j = 0; j < topo.getHeight(); j++) {
						computeNext(topo, i, j);
					}
				}
			}

		private:
			void computeNext(Topology& topo, int i, int j) {

#define POND	3

				unsigned char * previous = topo.getPrevious();
				int width = topo.getWidth();
				unsigned int blue  =  previous[3*(i+j*width) + Topology::BLUE];
				unsigned int green =  previous[3*(i+j*width) + Topology::GREEN];
				unsigned int red   =  previous[3*(i+j*width) + Topology::RED];

				int sign = 1 - 2 * (((red ^ green) >> 6)&1);
				int direction = ((green ^ blue) >> 3)&1;

				int d_i = sign * direction;
				int d_j = sign * (1-direction);

				int newBlue =  (blue  + POND*topo.getPrevious(i+d_i, j+d_j, Topology::BLUE ) + (POND+1)/2) /(POND+1);
				int newGreen = (green + POND*topo.getPrevious(i+d_i, j+d_j, Topology::GREEN) + (POND+1)/2) /(POND+1);
				int newRed =   (red   + POND*topo.getPrevious(i+d_i, j+d_j, Topology::RED  ) + (POND+1)/2) /(POND+1);

				topo.setCurrent(i, j, Topology::BLUE, newBlue);
				topo.setCurrent(i, j, Topology::GREEN, newGreen);
				topo.setCurrent(i, j, Topology::RED, newRed);
			}
		};

	};

}