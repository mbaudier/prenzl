#pragma once
#include "RuleFactory.h"
#include "Model/Util.h"
#include <math.h>
#include <fstream>

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
				createRelaxedLengthProjection();
			}


			void computeNext(Topology& topo) {
				for(int i = 0; i < topo.getWidth(); i++) {
					for(int j = 0; j < topo.getHeight(); j++) {
						computeNext(topo, i, j);
					}
				}
			}

		private:

			static const int gridInterval = 512;
			static const int springRelaxedLength = 512;
			static const int stiffness = 50;

			void createRelaxedLengthProjection() {
				int maxDeltaProjected = gridInterval + 256;
				int matrixDim = maxDeltaProjected/16;
				std::vector< std::pair<int,int> > prov(matrixDim);
				relaxedLengthProjection = std::vector< std::vector< std::pair< int, int> > >(matrixDim, prov);
				for(int i=0; i < matrixDim; i++) {
					for(int j=0; j < matrixDim; j++) {
						if(i==0) {
							relaxedLengthProjection[i][j].first = 0;
						}
						else {
							double ratio = (double) j * 16 / ((double) i * 16);
							double f_lx = (double)springRelaxedLength / sqrt(ratio*ratio + 1);
							relaxedLengthProjection[i][j].first = (int) f_lx;
						}

						if(j==0) {
							relaxedLengthProjection[i][j].second = 0;
						}
						else {
							double ratio = (double) i * 16 / ((double) j * 16);
							double f_ly = (double)springRelaxedLength / sqrt(ratio*ratio + 1);
							relaxedLengthProjection[i][j].second = (int) f_ly;
						}
					}
				}

#if 0
				std::ofstream out("debug.txt");
				for( int i=0; i < matrixDim; i++) {
					for( int j=0; j < matrixDim; j++) {
						out << relaxedLengthProjection[i][j].first << "\t";
					}
					out << std::endl;
				}
				out << std::endl;
				for( int i=0; i < matrixDim; i++) {
					for( int j=0; j < matrixDim; j++) {
						out << relaxedLengthProjection[i][j].second << "\t";
					}
					out << std::endl;
				}
				out.close();
#endif
			}

			void getRelaxedLengthProjection(int dx, int dy, int& l_x, int& l_y) {
				int dx_abs = (dx >= 0) ? dx : -dx;
				int dy_abs = (dy >= 0) ? dy : -dy;
				
#if 1
				// simplified
				if((dy_abs == 0) || (dx_abs/dy_abs >= 2)) {
					l_x = springRelaxedLength;
					l_y = 0;
				}
				else if((dx_abs == 0) || (dy_abs/dx_abs >= 2)) {
					l_y = springRelaxedLength;
					l_x = 0;
				}
				else {
					int diag = (springRelaxedLength * 724) / 1024;
					l_x = diag;
					l_y = diag;
				}
#else
				const std::pair<int, int>& proj = relaxedLengthProjection[dx_abs/16][dy_abs/16];
				l_x = proj.first;
				l_y = proj.second;
#endif
			}

			void addForces(int x, int y, int x_other, int y_other, int& f_x, int& f_y) {
				int dx = x_other - x;
				int dy = y_other - y;
				int l_x, l_y;
				getRelaxedLengthProjection(dx, dy, l_x, l_y);
				f_x += dx - ((dx >= 0) ? l_x : - l_x);
				f_y += dy - ((dy >= 0) ? l_y : - l_y);
			}

			void computeNext(Topology& topo, int i, int j) {
				unsigned char * previous = topo.getPrevious();
				unsigned char * anteprevious = topo.getAnteprevious();
				unsigned char * current = topo.getCurrent();
				int width = topo.getWidth();

				int f_x = 0;
				int f_y = 0;

				// center
				int c_x_pos = (int)topo.getPrevious(i, j, Topology::BLUE);
				int c_y_pos = (int)topo.getPrevious(i, j, Topology::GREEN);

				// North
				int n_x_pos = (int)topo.getPrevious(i, j-1, Topology::BLUE);
				int n_y_pos = (int)topo.getPrevious(i, j-1, Topology::GREEN);
				addForces(c_x_pos, c_y_pos, n_x_pos, n_y_pos - gridInterval, f_x, f_y);

				// South
				int s_x_pos = (int)topo.getPrevious(i, j+1, Topology::BLUE);
				int s_y_pos = (int)topo.getPrevious(i, j+1, Topology::GREEN);
				addForces(c_x_pos, c_y_pos, s_x_pos, s_y_pos + gridInterval, f_x, f_y);

				// West
				int w_x_pos = (int)topo.getPrevious(i-1, j, Topology::BLUE);
				int w_y_pos = (int)topo.getPrevious(i-1, j, Topology::GREEN);
				addForces(c_x_pos, c_y_pos, w_x_pos - gridInterval, w_y_pos, f_x, f_y);

				// East
				int e_x_pos = (int)topo.getPrevious(i+1, j, Topology::BLUE);
				int e_y_pos = (int)topo.getPrevious(i+1, j, Topology::GREEN);
				addForces(c_x_pos, c_y_pos, e_x_pos + gridInterval, e_y_pos, f_x, f_y);

				int old_x  =  anteprevious[I_BLUE (i,j,width)];
				int old_y  =  anteprevious[I_GREEN(i,j,width)];

				int new_x  = 2*c_x_pos - old_x + (f_x * stiffness)/1024;			
				int new_y  = 2*c_y_pos - old_y + (f_y * stiffness)/1024;

				current[I_BLUE (i,j,width)] = charSaturate(new_x);
				current[I_GREEN(i,j,width)] = charSaturate(new_y);

				int new_x_repos = new_x - 128;
				int new_y_repos = new_y - 128;
				current[I_RED  (i,j,width)] = charSaturate((new_x_repos*new_x_repos + new_y_repos*new_y_repos)/128);
			}

		private:
			std::vector<std::vector<std::pair<int,int> > > relaxedLengthProjection;

		};

	};

}