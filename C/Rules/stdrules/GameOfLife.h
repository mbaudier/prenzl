#pragma once
#include "Model/Rule.h"
#include "Model/Util.h"

namespace Prenzl {

	class GameOfLife : public Rule {
	public:
		
		inline void updateSums(Topology& topo, int i, int j, int& sumNeighborsAlive, int& sumColors) {
			int red = topo.getPrevious(i, j, Topology::RED);
			if(red > 127) { // neighboor alive
				sumNeighborsAlive++;
				sumColors += red << 16;
				sumColors += topo.getPrevious(i, j, Topology::GREEN) << 8;
				sumColors += topo.getPrevious(i, j, Topology::BLUE);
			}
		}

		void computeNext(Topology& topo, int i, int j) {
			bool alive = topo.getPrevious(i, j, Topology::RED) > 127;
			int sumNeighborsAlive = 0;
			int sumColors = 0;

			updateSums(topo, i-1, j-1, sumNeighborsAlive, sumColors);
			updateSums(topo, i-1, j+0, sumNeighborsAlive, sumColors);
			updateSums(topo, i-1, j+1, sumNeighborsAlive, sumColors);
			updateSums(topo, i+0, j-1, sumNeighborsAlive, sumColors);
			updateSums(topo, i+0, j+1, sumNeighborsAlive, sumColors);
			updateSums(topo, i+1, j-1, sumNeighborsAlive, sumColors);
			updateSums(topo, i+1, j+0, sumNeighborsAlive, sumColors);
			updateSums(topo, i+1, j+1, sumNeighborsAlive, sumColors);

			if((sumNeighborsAlive==3) || ((sumNeighborsAlive==2)&&alive)) {
				// cell is alive in current
				int average = sumColors / sumNeighborsAlive;
				topo.setCurrent(i, j, Topology::BLUE, average & 0x0000FF);
				topo.setCurrent(i, j, Topology::GREEN, (average & 0x00FF00) >> 8 );
				topo.setCurrent(i, j, Topology::RED, (average & 0xFF0000) >> 16 );
			}
			else {
				// cell is dead in current
				topo.setCurrent(i,j, Topology::BLUE, 30);
				topo.setCurrent(i,j, Topology::GREEN, 30);
				topo.setCurrent(i,j, Topology::RED, 30);
			}
		}

		void computeNext(Topology& topo) {
			for(int i = 0; i < topo.getWidth(); i++) {
				for(int j = 0; j < topo.getHeight(); j++) {
					computeNext(topo, i, j);
				}
			}
		}
	};

}
