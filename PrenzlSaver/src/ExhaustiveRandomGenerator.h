#pragma once
#include <vector>

namespace Prenzl {

	class ExhaustiveRandomGenerator {

	public:
		ExhaustiveRandomGenerator(size_t size = 1) 
			: accessed_(size, false)
			, nbAccessed(0)
		{}

		unsigned int next() {
			unsigned int size = (unsigned int)accessed_.size();
			if(nbAccessed == size) {
				// reinitialise
				accessed_ = std::vector<bool>(size, false);
				nbAccessed = 0;
			}
			unsigned int randomIncrement = rand() % size;
			unsigned int circularIndex = 0;
			unsigned int incr=0;
			while(true) {
				// find the next unaccessed file
				while(accessed_[circularIndex]) {
					incrementCircularIndex(circularIndex);
				}
				if(incr == randomIncrement) {
					accessed_[circularIndex] = true;
					nbAccessed++;
					return circularIndex;
				}
				incr++;
				incrementCircularIndex(circularIndex);
			}
		}

	private:
		void incrementCircularIndex(unsigned int& index) {
			index++;
			if(index >= accessed_.size()) {
				index -= (unsigned int)accessed_.size();
			}
		}

	private:
		std::vector<bool> accessed_;
		unsigned int nbAccessed;
	};

}