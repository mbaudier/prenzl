#pragma once
#include "Model/Rule.h"

namespace Prenzl {

	class Braque : public Rule {

	public:

		Braque( int volatility = 2, int shift = 2)
			: volatility(volatility)
			, shift(shift)
		{
			directions.push_back(std::pair<int,int>(-1,-1));
			directions.push_back(std::pair<int,int>(-1, 0));
			directions.push_back(std::pair<int,int>(-1, 1));
			directions.push_back(std::pair<int,int>( 0, 1));
			directions.push_back(std::pair<int,int>( 1, 1));
			directions.push_back(std::pair<int,int>( 1, 0));
			directions.push_back(std::pair<int,int>( 1,-1));
			directions.push_back(std::pair<int,int>( 0,-1));		
		}

		void computeNext(Topology& topo) {
			// compute all non border cells (optimised)
			unsigned char * current = topo.getCurrent();
			unsigned char * previous = topo.getPrevious();

			int width = topo.getWidth();
			int height = topo.getHeight();

			for(int i = 1; i < width - 1; i++) {
				for(int j = 1; j < height - 1; j++) {
					unsigned int blue  =  previous[I_BLUE(i,j,width) ];
					unsigned int green =  previous[I_GREEN(i,j,width)];
					unsigned int red   =  previous[I_RED(i,j,width)  ];

					const std::pair<int,int>& dir = directions[((red ^ green ^ blue)>>shift)&0x7];

					int d_i = dir.first;
					int d_j = dir.second;

					int newBlue =  (blue  + ((1<<volatility)-1) * previous[I_BLUE (i+d_i,j+d_j,width)] + (1<<(volatility-1))) >> volatility;
					int newGreen = (green + ((1<<volatility)-1) * previous[I_GREEN(i+d_i,j+d_j,width)] + (1<<(volatility-1))) >> volatility;
					int newRed =   (red   + ((1<<volatility)-1) * previous[I_RED  (i+d_i,j+d_j,width)] + (1<<(volatility-1))) >> volatility;

					current[I_BLUE(i,j,width) ] = newBlue;
					current[I_GREEN(i,j,width)] = newGreen;
					current[I_RED(i,j,width)  ] = newRed;
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
			unsigned char * current = topo.getCurrent();
			int width = topo.getWidth();
			unsigned int blue  =  previous[I_BLUE(i,j,width) ];
			unsigned int green =  previous[I_GREEN(i,j,width)];
			unsigned int red   =  previous[I_RED(i,j,width)  ];

			const std::pair<int,int>& dir = directions[((red ^ green ^ blue)>>shift)&0x7];

			int d_i = dir.first;
			int d_j = dir.second;

			int newBlue =  (blue  + ((1<<volatility)-1) * topo.getPrevious(i+d_i, j+d_j, Topology::BLUE ) + (1<<(volatility-1))) >> volatility;
			int newGreen = (green + ((1<<volatility)-1) * topo.getPrevious(i+d_i, j+d_j, Topology::GREEN) + (1<<(volatility-1))) >> volatility;
			int newRed =   (red   + ((1<<volatility)-1) * topo.getPrevious(i+d_i, j+d_j, Topology::RED  ) + (1<<(volatility-1))) >> volatility;

			current[I_BLUE(i,j,width) ] = newBlue;
			current[I_GREEN(i,j,width)] = newGreen;
			current[I_RED(i,j,width)  ] = newRed;
		}

	private: 
		int volatility;
		int shift;
		std::vector<std::pair<int,int> > directions;

	};

}
