#pragma once
#include "Model/Rule.h"

namespace Prenzl {

	class Lsd : public Rule {

	public:
		Lsd(unsigned char deltaB = 1, 
			unsigned char deltaG = 1, 
			unsigned char deltaR = 2,
			int dxB = -1, 
			int dyB = -1, 
			int dxG = 1, 
			int dyG = 0, 
			int dxR = 0, 
			int dyR = 1)
			: deltaB_(deltaB)
			, deltaG_(deltaG)
			, deltaR_(deltaR)
			, dxB_(dxB)
			, dyB_(dyB)
			, dxG_(dxG)
			, dyG_(dyG)
			, dxR_(dxR)
			, dyR_(dyR)
		{}

		void computeNext(Topology& topo) {
			// compute all non border cells (optimised)
			unsigned char * current = topo.getCurrent();
			unsigned char * previous = topo.getPrevious();

			int width = topo.getWidth();
			int height = topo.getHeight();

			for(int i = 1; i < width - 1; i++) {
				for(int j = 1; j < height - 1; j++) {
					current[I_BLUE(i,j,width) ] = previous[I_BLUE (i+dxB_,j+dyB_,width)] + deltaB_;
					current[I_GREEN(i,j,width)] = previous[I_GREEN(i+dxG_,j+dyG_,width)] + deltaG_;
					current[I_RED(i,j,width)  ] = previous[I_RED  (i+dxR_,j+dyR_,width)] + deltaR_;
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
			topo.setCurrent(i, j, Topology::BLUE , topo.getPrevious(i+dxB_, j+dyB_, Topology::BLUE ) + deltaB_);
			topo.setCurrent(i, j, Topology::GREEN, topo.getPrevious(i+dxG_, j+dyG_, Topology::GREEN) + deltaG_);
			topo.setCurrent(i, j, Topology::RED  , topo.getPrevious(i+dxR_, j+dyR_, Topology::RED  ) + deltaR_);
		}
	private:
		unsigned char deltaB_;
		unsigned char deltaG_;
		unsigned char deltaR_;

		int dxB_, dyB_, dxG_, dyG_, dxR_, dyR_;

	};

}
