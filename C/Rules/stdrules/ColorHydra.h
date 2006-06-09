#pragma once
#include "Model/Rule.h"

namespace Prenzl {

	class ColorHydra : public Rule {

	public:

		ColorHydra( int bodyLowerLimit = 300, int headLowerLimit = 550)
			: bodyLowerLimit(bodyLowerLimit)
			, headLowerLimit(headLowerLimit)
		{}

		void computeNext(Topology& topo) {
			// compute all non border cells (optimised)
			unsigned char * current = topo.getCurrent();
			unsigned char * previous = topo.getPrevious();

			int width = topo.getWidth();
			int height = topo.getHeight();

			for(int i = 1; i < width - 1; i++) {
				for(int j = 1; j < height - 1; j++) {
					int bC = previous[I_BLUE (i, j, width)]; 
					int gC = previous[I_GREEN(i, j, width)]; 
					int rC = previous[I_RED  (i, j, width)]; 

					int totalC = bC + gC + rC;

					int newBlue = 0;
					int newGreen = 0;
					int newRed = 0;

					if(totalC >= headLowerLimit) {
						// we have a head
						// a head normally dies and becomes a body

						// see what we have to loose (cumulated) to become a body
						int toLooseInTotal = totalC - headLowerLimit;

						// we share to lost among the 3 colors
						int toLoosePerColor = (toLooseInTotal + 3)/3;

						// apply
						newBlue = bC - toLoosePerColor;  if(newBlue < 0) {newBlue=0;}
						newGreen = gC - toLoosePerColor; if(newGreen < 0) {newGreen=0;}
						newRed = rC - toLoosePerColor;	 if(newRed < 0) {newRed=0;}
					}
					else if(totalC < bodyLowerLimit) {
						// we have a non animal
						// may become a head if 
						// - there is one and only one head around
						// - there is no body

						int bN = previous[I_BLUE (i + 0, j - 1, width)];
						int bS = previous[I_BLUE (i + 0, j + 1, width)];
						int bE = previous[I_BLUE (i + 1, j + 0, width)];
						int bW = previous[I_BLUE (i - 1, j + 0, width)];

						int gN = previous[I_GREEN(i + 0, j - 1, width)];
						int gS = previous[I_GREEN(i + 0, j + 1, width)];
						int gE = previous[I_GREEN(i + 1, j + 0, width)];
						int gW = previous[I_GREEN(i - 1, j + 0, width)];

						int rN = previous[I_RED  (i + 0, j - 1, width)];
						int rS = previous[I_RED  (i + 0, j + 1, width)];
						int rE = previous[I_RED  (i + 1, j + 0, width)];
						int rW = previous[I_RED  (i - 1, j + 0, width)];

						unsigned int nbHead = 0;
						unsigned int nbBody = 0;

						int totalN = bN + gN + rN;
						if(totalN >= headLowerLimit) {nbHead++;newBlue=bN;newGreen=gN;newRed=rN;}
						else if(totalN >= bodyLowerLimit) {nbBody++;}

						int totalS = bS + gS + rS;
						if(totalS >= headLowerLimit) {nbHead++;newBlue=bS;newGreen=gS;newRed=rS;}
						else if(totalS >= bodyLowerLimit) {nbBody++;}

						int totalE = bE + gE + rE;
						if(totalE >= headLowerLimit) {nbHead++;newBlue=bE;newGreen=gE;newRed=rE;}
						else if(totalE >= bodyLowerLimit) {nbBody++;}

						int totalW = bW + gW + rW;
						if(totalW >= headLowerLimit) {nbHead++;newBlue=bW;newGreen=gW;newRed=rW;}
						else if(totalW >= bodyLowerLimit) {nbBody++;}

						if((nbHead == 1)&&(nbBody==0)) {
							// become a head
							// let as it is
						}
						else {
							// blur with no correction (blur + a little bit darker)
							newBlue = (12*bC + bN + bS + bE + bW)/16;
							newGreen = (12*gC + gN + gS + gE + gW)/16;
							newRed = (12*rC + rN + rS + rE + rW)/16;
						}
					}
					else {
						// a body slowly returns to nature
						newBlue = bC - 1;  if(newBlue < 0) {newBlue=0;}
						newGreen = gC - 1; if(newGreen < 0) {newGreen=0;} 
						newRed = rC - 1;   if(newRed < 0) {newRed=0;}
					}

					current[I_BLUE (i, j, width)] = newBlue;
					current[I_GREEN(i, j, width)] = newGreen;
					current[I_RED  (i, j, width)] = newRed;
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
			unsigned char * previous = topo.getPrevious();
			int width = topo.getWidth();

			// for blue
			int bC = previous[I_BLUE (i, j, width)]; 
			int gC = previous[I_GREEN(i, j, width)]; 
			int rC = previous[I_RED  (i, j, width)]; 

			int totalC = bC + gC + rC;

			int newBlue = 0;
			int newGreen = 0;
			int newRed = 0;

			if(totalC >= headLowerLimit) {
				// we have a head
				// a head normally dies and becomes a body

				// see what we have to loose (cumulated) to become a body
				int toLooseInTotal = totalC - headLowerLimit;

				// we share to lost among the 3 colors
				int toLoosePerColor = (toLooseInTotal + 3)/3;

				// apply
				newBlue = bC - toLoosePerColor;  if(newBlue < 0) {newBlue=0;}
				newGreen = gC - toLoosePerColor; if(newGreen < 0) {newGreen=0;}
				newRed = rC - toLoosePerColor;	 if(newRed < 0) {newRed=0;}
			}
			else if(totalC < bodyLowerLimit) {
				// we have a non animal
				// may become a head if 
				// - there is one and only one head around
				// - there is no body

				int bN = topo.getPrevious(i + 0, j - 1, Topology::BLUE);
				int bS = topo.getPrevious(i + 0, j + 1, Topology::BLUE);
				int bE = topo.getPrevious(i + 1, j + 0, Topology::BLUE);
				int bW = topo.getPrevious(i - 1, j + 0, Topology::BLUE);

				int gN = topo.getPrevious(i + 0, j - 1, Topology::GREEN);
				int gS = topo.getPrevious(i + 0, j + 1, Topology::GREEN);
				int gE = topo.getPrevious(i + 1, j + 0, Topology::GREEN);
				int gW = topo.getPrevious(i - 1, j + 0, Topology::GREEN);

				int rN = topo.getPrevious(i + 0, j - 1, Topology::RED);
				int rS = topo.getPrevious(i + 0, j + 1, Topology::RED);
				int rE = topo.getPrevious(i + 1, j + 0, Topology::RED);
				int rW = topo.getPrevious(i - 1, j + 0, Topology::RED);

				unsigned int nbHead = 0;
				unsigned int nbBody = 0;

				int totalN = bN + gN + rN;
				if(totalN >= headLowerLimit) {nbHead++;newBlue=bN;newGreen=gN;newRed=rN;}
				else if(totalN >= bodyLowerLimit) {nbBody++;}

				int totalS = bS + gS + rS;
				if(totalS >= headLowerLimit) {nbHead++;newBlue=bS;newGreen=gS;newRed=rS;}
				else if(totalS >= bodyLowerLimit) {nbBody++;}

				int totalE = bE + gE + rE;
				if(totalE >= headLowerLimit) {nbHead++;newBlue=bE;newGreen=gE;newRed=rE;}
				else if(totalE >= bodyLowerLimit) {nbBody++;}

				int totalW = bW + gW + rW;
				if(totalW >= headLowerLimit) {nbHead++;newBlue=bW;newGreen=gW;newRed=rW;}
				else if(totalW >= bodyLowerLimit) {nbBody++;}

				if((nbHead == 1)&&(nbBody==0)) {
					// become a head
					// let as it is
				}
				else {
					// blur with no correction (blur + a little bit darker)
					newBlue = (12*bC + bN + bS + bE + bW)/16;
					newGreen = (12*gC + gN + gS + gE + gW)/16;
					newRed = (12*rC + rN + rS + rE + rW)/16;
				}
			}
			else {
				// a body slowly returns to nature
				newBlue = bC - 1;  if(newBlue < 0) {newBlue=0;}
				newGreen = gC - 1; if(newGreen < 0) {newGreen=0;} 
				newRed = rC - 1;   if(newRed < 0) {newRed=0;}
			}

			current[I_BLUE (i, j, width)] = newBlue;
			current[I_GREEN(i, j, width)] = newGreen;
			current[I_RED  (i, j, width)] = newRed;
		}

	private: 
		int bodyLowerLimit;
		int headLowerLimit;

	};

}
