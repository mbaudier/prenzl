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
			{
			}


			void computeNext(Topology& topo) {
				for(int i = 0; i < topo.getWidth(); i++) {
					for(int j = 0; j < topo.getHeight(); j++) {
						computeNext(topo, i, j);
					}
				}
			}

		private:
	
#if 0 /*phase cheulou avec zones grises*/
			static const int coeffBG = 20;
			static const int coeffBR = 0;
			static const int coeffGR = 20;
			static const int coeffGB = 0;
			static const int coeffRB = -20;
			static const int coeffRG = 0;
			static const int coeffBB = 0;
			static const int coeffGG = 0;
			static const int coeffRR = 0;
#elif 0 /*bof*/
			static const int coeffBG = 20;
			static const int coeffBR = -10;
			static const int coeffGR = 20;
			static const int coeffGB = -10;
			static const int coeffRB = -20;
			static const int coeffRG = 10;
			static const int coeffBB = 0;
			static const int coeffGG = 0;
			static const int coeffRR = 0;
#elif 0 /*sympa*/
			static const int coeffBG = -20;
			static const int coeffBR = 0;
			static const int coeffGR = -20;
			static const int coeffGB = 0;
			static const int coeffRB = -20;
			static const int coeffRG = 0;
			static const int coeffBB = 10;
			static const int coeffGG = 10;
			static const int coeffRR = 10;
#elif 1 /*type phaseCheulou squares*/
			static const int coeffBG =  20;
			static const int coeffBR =  20;
			static const int coeffGR =  20;
			static const int coeffGB =  20;
			static const int coeffRB = -10;
			static const int coeffRG = -10;
			static const int coeffBB =   0;
			static const int coeffGG =   0;
			static const int coeffRR =   0;
#elif 0
			static const int coeffBG =  90;
			static const int coeffBR =  90;
			static const int coeffGR =  90;
			static const int coeffGB =  90;
			static const int coeffRB = -90;
			static const int coeffRG = -90;
			static const int coeffBB =   0;
			static const int coeffGG =   0;
			static const int coeffRR =   0;
#else
			static const int coeffBG =  90;
			static const int coeffBR =  90;
			static const int coeffGR =  90;
			static const int coeffGB =  90;
			static const int coeffRB = -90;
			static const int coeffRG = -90;
			static const int coeffBB =   0;
			static const int coeffGG =   0;
			static const int coeffRR =   0;
#endif

			void computeNext(Topology& topo, int i, int j) {
				unsigned char * previous = topo.getPrevious();
				unsigned char * anteprevious = topo.getAnteprevious();
				unsigned char * current = topo.getCurrent();
				int width = topo.getWidth();

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
				int meansR = (totalR + 4) / 9 - 127;
				int meansG = (totalG + 4) / 9 - 127;
				int meansB = (totalB + 4) / 9 - 127;

				int blue  =  previous[I_BLUE (i,j,width)];
				int green =  previous[I_GREEN(i,j,width)];
				int red   =  previous[I_RED  (i,j,width)];

				int oldBlue  =  anteprevious[I_BLUE (i,j,width)];
				int oldGreen =  anteprevious[I_GREEN(i,j,width)];
				int oldRed   =  anteprevious[I_RED  (i,j,width)];

				blue  = 2*blue  - oldBlue  + (coeffBG*meansG + coeffBR*meansR + coeffBB*meansB)/1024;			
				green = 2*green - oldGreen + (coeffGR*meansR + coeffGB*meansB + coeffGG*meansG)/1024;
				red   = 2*red   - oldRed   + (coeffRB*meansB + coeffRG*meansG + coeffRR*meansR)/1024;

				current[I_BLUE (i,j,width)] = charSaturate(blue);
				current[I_GREEN(i,j,width)] = charSaturate(green);
				current[I_RED  (i,j,width)] = charSaturate(red);
			}

		};

	};

}