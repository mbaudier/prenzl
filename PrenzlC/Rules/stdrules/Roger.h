#pragma once
#include "Model/Rule.h"

namespace Prenzl {


	class Roger : public Rule {
	public:
		
		Roger() 
		{}

		void computeNext(Topology& topo, int i, int j) {
		
			int third = Topology::BLUE;
			int second = Topology::GREEN;
			int leader = Topology::RED;
		
			int oldblue = topo.getPrevious(i + 0, j + 0, leader);
			int totalB 	= (topo.getPrevious(i + 0, j + 1, leader)>>7)
						+ (topo.getPrevious(i + 0, j - 1, leader)>>7)
						+ (topo.getPrevious(i + 1, j + 0, leader)>>7)
						+ (topo.getPrevious(i - 1, j + 0, leader)>>7)
						+ (oldblue>>7);
			
/*			char color = (totalB & 1) ? 128 : 0
				+ (oldblue & 0x7F)
				+ 1;
*/			
			char color = (totalB & 1) ? 255 : 0;
			
			int mod = (totalB & 1) ? 1 : -1;
			
			
			
			topo.setCurrent(i, j, leader, color);
			
			int oldgren = topo.getPrevious(i, j, second);
			topo.setCurrent(i, j, second, (char) (oldgren + mod));
			
			int oldred = topo.getPrevious(i, j, third);
			topo.setCurrent(i, j, third, (char) (oldred - mod));
			
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
