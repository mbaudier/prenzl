#pragma once
#include "Model/Rule.h"
#include "Model/Util.h"

namespace Prenzl {

	class ChannelPropagation : public Rule {

	public:

		ChannelPropagation(int cB = 30, int cG = 30, int cR = 30, int colorInteraction = 4)
			: cB(cB)
			, cG(cG)
			, cR(cR)
			, colorInteraction(colorInteraction)
		{}

		void computeNext(Topology& topo) {
			// compute all non border cells (optimised)
			unsigned char * previous = topo.getPrevious();
			unsigned char * anteprevious = topo.getAnteprevious();
			unsigned char * current = topo.getCurrent();

			int width = topo.getWidth();
			int height = topo.getHeight();

			for(int i = 1; i < width - 1; i++) {
				for(int j = 1; j < height - 1; j++) {

					int oldBlue = previous[I_BLUE(i,j,width)];
					int l_b = (int)previous[I_BLUE(i+1,j+0,width)]
							+ (int)previous[I_BLUE(i-1,j+0,width)]
							+ (int)previous[I_BLUE(i+0,j+1,width)]
							+ (int)previous[I_BLUE(i+0,j-1,width)]
							- 4 * oldBlue;

					int blue = 2 * oldBlue - anteprevious[I_BLUE(i,j,width)] + (cB*l_b)/1024;

					int oldGreen = previous[I_GREEN(i,j,width)];
					int l_g = (int)previous[I_GREEN(i+1,j+0,width)]
							+ (int)previous[I_GREEN(i-1,j+0,width)]
							+ (int)previous[I_GREEN(i+0,j+1,width)]
							+ (int)previous[I_GREEN(i+0,j-1,width)]
							- 4 * oldGreen;

					int green = 2 * oldGreen - anteprevious[I_GREEN(i,j,width)] + (cG*l_g)/1024;

					int oldRed = previous[I_RED(i,j,width)];
					int l_r = (int)previous[I_RED(i+1,j+0,width)]
							+ (int)previous[I_RED(i-1,j+0,width)]
							+ (int)previous[I_RED(i+0,j+1,width)]
							+ (int)previous[I_RED(i+0,j-1,width)]
							- 4 * oldRed;

					int red = 2 * oldRed - anteprevious[I_RED(i,j,width)] + (cR*l_r)/1024;

					// color interaction
					if(colorInteraction) {
						if(red<=0)   blue += (green<=0) ? +colorInteraction : -colorInteraction;
						if(blue<=0)  green += (red<=0) ? +colorInteraction: -colorInteraction;
						if(green<=0) red += (blue <=0) ? +colorInteraction : -colorInteraction;
					}

					current[I_BLUE(i,j,width)] = saturate(blue, 0, 255);
					current[I_GREEN(i,j,width)] = saturate(green, 0, 255);
					current[I_RED(i,j,width)] = saturate(red, 0, 255);					
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
			unsigned char * previous = topo.getPrevious();
			unsigned char * anteprevious = topo.getAnteprevious();
			unsigned char * current = topo.getCurrent();

			int width = topo.getWidth();

			int oldBlue = previous[I_BLUE(i,j,width)];
			int l_b =   (int)topo.getPrevious(i+1,j+0, Topology::BLUE)
					    + (int)topo.getPrevious(i-1,j+0, Topology::BLUE)
					    + (int)topo.getPrevious(i+0,j+1, Topology::BLUE)
					    + (int)topo.getPrevious(i+0,j-1, Topology::BLUE)
						- 4 * oldBlue;

			int blue = 2 * oldBlue - anteprevious[I_BLUE(i,j,width)] + (cB*l_b)/1024;

			int oldGreen = previous[I_GREEN(i,j,width)];
			int l_g =   (int)topo.getPrevious(i+1,j+0, Topology::GREEN)
					    + (int)topo.getPrevious(i-1,j+0, Topology::GREEN)
					    + (int)topo.getPrevious(i+0,j+1, Topology::GREEN)
					    + (int)topo.getPrevious(i+0,j-1, Topology::GREEN)
						- 4 * oldGreen;

			int green = 2 * oldGreen - anteprevious[I_GREEN(i,j,width)] + (cG*l_g)/1024;

			int oldRed = previous[I_RED(i,j,width)];
			int l_r =   (int)topo.getPrevious(i+1,j+0, Topology::RED)
					    + (int)topo.getPrevious(i-1,j+0, Topology::RED)
					    + (int)topo.getPrevious(i+0,j+1, Topology::RED)
					    + (int)topo.getPrevious(i+0,j-1, Topology::RED)
						- 4 * oldRed;

			int red = 2 * oldRed - anteprevious[I_RED(i,j,width)] + (cR*l_r)/1024;

			// color interaction
			if(colorInteraction) {
				if(red<=0)   blue += (green<=0) ? +colorInteraction : -colorInteraction;
				if(blue<=0)  green += (red<=0) ? +colorInteraction: -colorInteraction;
				if(green<=0) red += (blue <=0) ? +colorInteraction : -colorInteraction;
			}

			current[I_BLUE(i,j,width)] = saturate(blue, 0, 255);
			current[I_GREEN(i,j,width)] = saturate(green, 0, 255);
			current[I_RED(i,j,width)] = saturate(red, 0, 255);
		}

	private: 
		int cB, cG, cR;
		int colorInteraction;

	};

}
