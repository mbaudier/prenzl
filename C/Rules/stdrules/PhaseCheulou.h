#pragma once
#include "Model/Rule.h"
#include "Model/Util.h"

namespace Prenzl {

	//! Phase Cheulou Rule
	/*!
		Differential equations by color
	*/
	class PhaseCheulou : public Rule {
	public:
		
		PhaseCheulou(	int coeffBG = 90,
						int coeffBR = 0,
						int coeffGR = 90,
						int coeffGB = 0,
						int coeffRB = -90,
						int coeffRG = 0,
						int coeffBB = 0,
						int coeffGG = 0,
						int coeffRR = 0)
			: coeffBG(coeffBG)
			, coeffBR(coeffBR)
			, coeffGR(coeffGR)
			, coeffGB(coeffGB)
			, coeffRB(coeffRB)
			, coeffRG(coeffRG)
			, coeffBB(coeffBB)
			, coeffGG(coeffGG)
			, coeffRR(coeffRR)
		{
			// init divByNine array
			for(int i = 0; i < 9*256; i++) {
				divByNine[i] = (i + 4) / 9;
			}


		}

		inline void updateValues(int& blue, int& green, int& red, int totalB, int totalG, int totalR) {
#if 0
			int meansR = (totalR + 4) / 9 - 127;
			int meansG = (totalG + 4) / 9 - 127;
			int meansB = (totalB + 4) / 9 - 127;

			blue  += (coeffBG*meansG + coeffBR*meansR + coeffBB*meansB)>>10;			
			green += (coeffGR*meansR + coeffGB*meansB + coeffGG*meansG)>>10;
			red   += (coeffRB*meansB + coeffRG*meansG + coeffRR*meansR)>>10;
#else /*optimised*/
			int meansR = (int)divByNine[totalR] - 127;
			int meansG = (int)divByNine[totalG] - 127;
			int meansB = (int)divByNine[totalB] - 127;

			blue  += (coeffBG*meansG + coeffBR*meansR + coeffBB*meansB)>>10;			
			green += (coeffGR*meansR + coeffGB*meansB + coeffGG*meansG)>>10;
			red   += (coeffRB*meansB + coeffRG*meansG + coeffRR*meansR)>>10;
#endif

		}

		//! Local computeNext
		/*!
			Used for borders
		*/
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

			int blue  =  topo.getPrevious(i, j, Topology::BLUE);
			int green =  topo.getPrevious(i, j, Topology::GREEN); 
			int red   =  topo.getPrevious(i, j, Topology::RED) ; 

			updateValues(blue, green, red, totalB, totalG, totalR);

			topo.setCurrent(i, j, Topology::BLUE,  (unsigned char)(charSaturate(blue)));
			topo.setCurrent(i, j, Topology::GREEN, (unsigned char)(charSaturate(green)));
			topo.setCurrent(i, j, Topology::RED,   (unsigned char)(charSaturate(red)));
		}

		//! Gobal computeNext
		void computeNext(Topology& topo) {

			// compute all non border cells (optimised)
			unsigned char * current = topo.getCurrent();
			unsigned char * previous = topo.getPrevious();

			int width = topo.getWidth();
			int height = topo.getHeight();

			for(int i = 1; i < width - 1; i++) {
				for(int j = 1; j < height - 1; j++) {
					int blue  =  previous[3*(i+j*width) + Topology::BLUE];
					int green =  previous[3*(i+j*width) + Topology::GREEN];
					int red   =  previous[3*(i+j*width) + Topology::RED];

					int totalR = previous[3*((i-1)+(j-1)*width) + Topology::RED]
							   + previous[3*((i-1)+(j+0)*width) + Topology::RED]
							   + previous[3*((i-1)+(j+1)*width) + Topology::RED]
							   + previous[3*((i+0)+(j-1)*width) + Topology::RED]
							   + previous[3*((i+0)+(j+1)*width) + Topology::RED]
							   + previous[3*((i+1)+(j-1)*width) + Topology::RED]
							   + previous[3*((i+1)+(j+0)*width) + Topology::RED]
							   + previous[3*((i+1)+(j+1)*width) + Topology::RED]
							   + red ;

					int totalG = previous[3*((i-1)+(j-1)*width) + Topology::GREEN]
							   + previous[3*((i-1)+(j+0)*width) + Topology::GREEN]
							   + previous[3*((i-1)+(j+1)*width) + Topology::GREEN]
							   + previous[3*((i+0)+(j-1)*width) + Topology::GREEN]
							   + previous[3*((i+0)+(j+1)*width) + Topology::GREEN]
							   + previous[3*((i+1)+(j-1)*width) + Topology::GREEN]
							   + previous[3*((i+1)+(j+0)*width) + Topology::GREEN]
							   + previous[3*((i+1)+(j+1)*width) + Topology::GREEN]
							   + green ;

					int totalB = previous[3*((i-1)+(j-1)*width) + Topology::BLUE]
							   + previous[3*((i-1)+(j+0)*width) + Topology::BLUE]
							   + previous[3*((i-1)+(j+1)*width) + Topology::BLUE]
							   + previous[3*((i+0)+(j-1)*width) + Topology::BLUE]
							   + previous[3*((i+0)+(j+1)*width) + Topology::BLUE]
							   + previous[3*((i+1)+(j-1)*width) + Topology::BLUE]
							   + previous[3*((i+1)+(j+0)*width) + Topology::BLUE]
							   + previous[3*((i+1)+(j+1)*width) + Topology::BLUE]
							   + blue ;

					updateValues(blue, green, red, totalB, totalG, totalR);

					current[3*(i+j*width) + Topology::BLUE] = (unsigned char)(charSaturate(blue));
					current[3*(i+j*width) + Topology::GREEN] = (unsigned char)(charSaturate(green));
					current[3*(i+j*width) + Topology::RED] = (unsigned char)(charSaturate(red));

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
		int coeffBG;
		int coeffBR;
		int coeffGR;
		int coeffGB;
		int coeffRB;
		int coeffRG;
		int coeffBB;
		int coeffGG;
		int coeffRR;

		//! For optimisation
		unsigned char divByNine[9*256];


	};

}
