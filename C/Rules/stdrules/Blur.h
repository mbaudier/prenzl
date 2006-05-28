#pragma once
#include "Model/Rule.h"

namespace Prenzl {

	class Blur : public Rule {
	public:
		
		void computeNext(Topology& topo, int i, int j) {
			int totalR = 0;
			int totalG = 0;
			int totalB = 0;
			for(int di = -1; di <= 1; di++) {
				for(int dj = -1; dj <= 1; dj++) {
					totalB +=  topo.getPrevious(i + di, j + dj, Topology::BLUE);
					totalG +=  topo.getPrevious(i + di, j + dj, Topology::GREEN); 
					totalR +=  topo.getPrevious(i + di, j + dj, Topology::RED) ; 
				}
			}

			topo.setCurrent(i, j, Topology::BLUE, (char)((totalB + 5)/9));
			topo.setCurrent(i, j, Topology::GREEN, (char)((totalG + 5)/9));
			topo.setCurrent(i, j, Topology::RED, (char)((totalR + 5)/9 ));
		}


		void computeNext(Topology& topo) {
#if 0 /*optimisation*/
			for(int i = 0; i < topo.getWidth(); i++) {
				for(int j = 0; j < topo.getHeight(); j++) {
					computeNext(topo, i, j);
				}
			}
#else
			unsigned char * current = topo.getCurrent();
			unsigned char * previous = topo.getPrevious();

			int width = topo.getWidth();
			int height = topo.getHeight();

			for(int i = 1; i < width - 1; i++) {
				for(int j = 1; j < height - 1; j++) {
					int totalR = previous[I_RED(i-1,j-1,width)]
							   + previous[I_RED(i-1,j+0,width)]
							   + previous[I_RED(i-1,j+1,width)]
							   + previous[I_RED(i+0,j-1,width)]
							   + previous[I_RED(i+0,j+0,width)]
							   + previous[I_RED(i+0,j+1,width)]
							   + previous[I_RED(i+1,j-1,width)]
							   + previous[I_RED(i+1,j+0,width)]
							   + previous[I_RED(i+1,j+1,width)];

					int totalG = previous[I_GREEN(i-1,j-1,width)]
							   + previous[I_GREEN(i-1,j+0,width)]
							   + previous[I_GREEN(i-1,j+1,width)]
							   + previous[I_GREEN(i+0,j-1,width)]
							   + previous[I_GREEN(i+0,j+0,width)]
							   + previous[I_GREEN(i+0,j+1,width)]
							   + previous[I_GREEN(i+1,j-1,width)]
							   + previous[I_GREEN(i+1,j+0,width)]
							   + previous[I_GREEN(i+1,j+1,width)];

					int totalB = previous[I_BLUE(i-1,j-1,width)]
							   + previous[I_BLUE(i-1,j+0,width)]
							   + previous[I_BLUE(i-1,j+1,width)]
							   + previous[I_BLUE(i+0,j-1,width)]
							   + previous[I_BLUE(i+0,j+0,width)]
							   + previous[I_BLUE(i+0,j+1,width)]
							   + previous[I_BLUE(i+1,j-1,width)]
							   + previous[I_BLUE(i+1,j+0,width)]
							   + previous[I_BLUE(i+1,j+1,width)];

					current[I_BLUE(i,j,width)] = (unsigned char)((totalB+5)/9);
					current[I_GREEN(i,j,width)] = (unsigned char)((totalG+5)/9);
					current[I_RED(i,j,width)] = (unsigned char)((totalR+5)/9);
				}
			}
			
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


#endif
		}

	};

}
