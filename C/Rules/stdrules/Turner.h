#pragma once
#include "Model/Rule.h"

namespace Prenzl {

	class Turner : public Rule {

	public:

		Turner( int volatility = 2, int sign_bit = 3, int dir_bit = 4)
			: volatility(volatility)
			, sign_bit(sign_bit)
			, dir_bit(dir_bit)
		{}

		void computeNext(Topology& topo) {
			// compute all non border cells (optimised)
			unsigned char * current = topo.getCurrent();
			unsigned char * previous = topo.getPrevious();

			int width = topo.getWidth();
			int height = topo.getHeight();

			for(int i = 1; i < width - 1; i++) {
				for(int j = 1; j < height - 1; j++) {
					unsigned int blue  =  previous[3*(i+j*width) + Topology::BLUE ];
					unsigned int green =  previous[3*(i+j*width) + Topology::GREEN];
					unsigned int red   =  previous[3*(i+j*width) + Topology::RED  ];

					int sign = 1 - 2 * ((green >> sign_bit)&1);
					int direction = (red >> dir_bit)&1;

					int d_i = sign * direction;
					int d_j = sign * (1-direction);

					int newBlue =  (blue  + ((1<<volatility)-1) * previous[3*((i+d_i)+(j+d_j)*width)+ Topology::BLUE ] + (1<<(volatility-1))) >> volatility;

					sign = 1 - 2 * ((red >> sign_bit)&1);
					direction = (blue >> dir_bit)&1;

					d_i = sign * direction;
					d_j = sign * (1-direction);

					int newGreen = (green + ((1<<volatility)-1) * previous[3*((i+d_i)+(j+d_j)*width)+ Topology::GREEN] + (1<<(volatility-1))) >> volatility;

					sign = 1 - 2 * ((blue >> sign_bit)&1);
					direction = (green >> dir_bit)&1;

					d_i = sign * direction;
					d_j = sign * (1-direction);

					int newRed =   (red   + ((1<<volatility)-1) * previous[3*((i+d_i)+(j+d_j)*width)+ Topology::RED  ] + (1<<(volatility-1))) >> volatility;

					current[3*(i+j*width) + Topology::BLUE ] = newBlue;
					current[3*(i+j*width) + Topology::GREEN] = newGreen;
					current[3*(i+j*width) + Topology::RED  ] = newRed;
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
			int width = topo.getWidth();
			unsigned int blue  =  previous[3*(i+j*width) + Topology::BLUE];
			unsigned int green =  previous[3*(i+j*width) + Topology::GREEN];
			unsigned int red   =  previous[3*(i+j*width) + Topology::RED];

			int sign = 1 - 2 * ((green >> sign_bit)&1);
			int direction = (red >> dir_bit)&1;

			int d_i = sign * direction;
			int d_j = sign * (1-direction);

			int newBlue =  (blue  + ((1<<volatility)-1) * topo.getPrevious(i+d_i, j+d_j, Topology::BLUE ) + (1<<(volatility-1))) >> volatility;

			sign = 1 - 2 * ((red >> sign_bit)&1);
			direction = (blue >> dir_bit)&1;

			d_i = sign * direction;
			d_j = sign * (1-direction);

			int newGreen = (green + ((1<<volatility)-1) * topo.getPrevious(i+d_i, j+d_j, Topology::GREEN) + (1<<(volatility-1))) >> volatility;

			sign = 1 - 2 * ((blue >> sign_bit)&1);
			direction = (green >> dir_bit)&1;

			d_i = sign * direction;
			d_j = sign * (1-direction);

			int newRed =   (red   + ((1<<volatility)-1) * topo.getPrevious(i+d_i, j+d_j, Topology::RED  ) + (1<<(volatility-1))) >> volatility;

			topo.setCurrent(i, j, Topology::BLUE, newBlue);
			topo.setCurrent(i, j, Topology::GREEN, newGreen);
			topo.setCurrent(i, j, Topology::RED, newRed);
		}

	private: 
		int volatility;
		int sign_bit;
		int dir_bit;

	};

}
