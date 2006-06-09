#pragma once
#include "Model/Rule.h"

namespace Prenzl {

	class Hydra : public Rule {

	public:

		Hydra( int bodyLowerLimit = 100, int headLowerLimit = 200)
			: bodyLowerLimit(bodyLowerLimit)
			, headLowerLimit(headLowerLimit)
		{}

		inline unsigned char getNewValue(unsigned char self, unsigned char north, unsigned char south,
									unsigned char east, unsigned char west)
		{
			if(self >= headLowerLimit) {
				// we have a head
				// a head always dies and becomes a body
				return bodyLowerLimit - 1;
			}
			else if(self < bodyLowerLimit ) {
				// we have non animal
				// may become a head if 
				// - there is one and only one head around
				// - there is no body
				unsigned int nbHead = 0;
				unsigned int nbBody = 0;
				if(north >= headLowerLimit) {nbHead++;}
				else if(north >= bodyLowerLimit) {nbBody++;}
				if(south >= headLowerLimit) {nbHead++;}
				else if(south >= bodyLowerLimit) {nbBody++;}
				if(east >= headLowerLimit) {nbHead++;}
				else if(east >= bodyLowerLimit) {nbBody++;}
				if(west >= headLowerLimit) {nbHead++;}
				else if(west >= bodyLowerLimit) {nbBody++;}

				if((nbHead == 1)&&(nbBody==0)) {
					return headLowerLimit;
				}
				else {
					// blur with no correction (blur + a little bit darker)
					return (4*self + north + south + east + west)/8;
				}
			}
			else {
				// a body slowly returns to nature
				return self - 1;
			}
		}

		void computeNext(Topology& topo) {
			// compute all non border cells (optimised)
			unsigned char * current = topo.getCurrent();
			unsigned char * previous = topo.getPrevious();

			int width = topo.getWidth();
			int height = topo.getHeight();

			for(int i = 1; i < width - 1; i++) {
				for(int j = 1; j < height - 1; j++) {
					unsigned char north = previous[I_BLUE(i + 0, j - 1, width)];
					unsigned char south = previous[I_BLUE(i + 0, j + 1, width)];
					unsigned char east  = previous[I_BLUE(i + 1, j + 0, width)];
					unsigned char west  = previous[I_BLUE(i - 1, j + 0, width)];
					unsigned char self  = previous[I_BLUE(i + 0, j + 0, width)];

					unsigned char newBlue = getNewValue(self, north, south, east, west);

					current[I_BLUE (i, j, width)] = newBlue;
					current[I_GREEN(i, j, width)] = newBlue;
					current[I_RED  (i, j, width)] = newBlue;
				}
			}

			// compute border cells (non optimised)

			// top and bottom line
			for(int i = 0; i < width; i++) {
				this->computeNext(topo, i, 0);
				this->computeNext(topo, i, height - 1);
			}

			// left and right line
			for(int j = 1; j < height - 1; j++) {
				this->computeNext(topo, 0, j);
				this->computeNext(topo, width - 1, j);
			}
		}

	private:
		void computeNext(Topology& topo, int i, int j) {
			unsigned char * current = topo.getCurrent();
			int width = topo.getWidth();

			unsigned char north = topo.getPrevious(i + 0, j - 1, Topology::BLUE);
			unsigned char south = topo.getPrevious(i + 0, j + 1, Topology::BLUE);
			unsigned char east  = topo.getPrevious(i + 1, j + 0, Topology::BLUE);
			unsigned char west  = topo.getPrevious(i - 1, j + 0, Topology::BLUE);
			unsigned char self  = topo.getPrevious(i + 0, j + 0, Topology::BLUE);

			unsigned char newBlue = getNewValue(self, north, south, east, west);

			current[I_BLUE (i, j, width)] = newBlue;
			current[I_GREEN(i, j, width)] = newBlue;
			current[I_RED  (i, j, width)] = newBlue;
		}

	private: 
		int bodyLowerLimit;
		int headLowerLimit;

	};

}
