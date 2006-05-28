#pragma once

#ifndef _WINDOWS
#include <jni.h>
#else
#define JNIEXPORT
#endif 

#include <string.h>
#include "Util.h"

// Macros for indexes (unsafe access)
#define I_BASIS(_I_, _J_, _WIDTH_)		( 3 * ( (_I_) + (_J_) * (_WIDTH_) ) )
#define I_BLUE(_I_, _J_, _WIDTH_)		( I_BASIS(_I_, _J_, _WIDTH_) + Topology::BLUE )
#define I_GREEN(_I_, _J_, _WIDTH_)		( I_BASIS(_I_, _J_, _WIDTH_) + Topology::GREEN )
#define I_RED(_I_, _J_, _WIDTH_)		( I_BASIS(_I_, _J_, _WIDTH_) + Topology::RED )

namespace Prenzl {

	class Topology {
	public:
		JNIEXPORT Topology(int width, int height) 
			: width_(width)
			, height_(height)
		{
			anteprevious_ = new unsigned char[width*height*3];
			previous_ = new unsigned char[width*height*3];
			current_ = new unsigned char[width*height*3];
		}

		JNIEXPORT Topology(int width, int height,unsigned char* firstGen)
			: width_(width)
			, height_(height)
		{
			anteprevious_ = new unsigned char[width*height*3];
			previous_ = new unsigned char[width*height*3];
			current_ = new unsigned char[width*height*3];
			memcpy(current_, firstGen, width*height*3);
			memcpy(previous_, firstGen, width*height*3);
		}

		JNIEXPORT ~Topology() {
			delete[] anteprevious_;
			delete[] previous_;
			delete[] current_;
		}

		int getPosition(int i, int j) {
			i = quickModulo(i, this->width_);
			j = quickModulo(j, this->height_);

			return 3*(i + j * this->width_);
		}

		unsigned char getAnteprevious(int i, int j, int color) {
			return this->anteprevious_[getPosition(i, j) + color];
		}

		unsigned char getPrevious(int i, int j, int color) {
			return this->previous_[getPosition(i, j) + color];
		}

		JNIEXPORT void Topology::setCurrent(int i, int j, int color, unsigned char value) {
			this->current_[getPosition(i, j) + color] = value;
		}

		void nextGeneration() {
			unsigned char * temp = previous_;
			previous_ = current_;
			current_ = anteprevious_;
			anteprevious_ = temp;
		}

		int getWidth() {
			return width_;
		}

		int getHeight() {
			return height_;
		}

		unsigned char * getCurrent() {
			return current_;
		}

		unsigned char * getPrevious() {
			return previous_;
		}

		unsigned char * getAnteprevious() {
			return anteprevious_;
		}
		
		static const int BLUE = 0;
		static const int GREEN = 1;
		static const int RED = 2;

	protected:

		unsigned char * anteprevious_;
		unsigned char * previous_;
		unsigned char * current_;

		int width_;
		int height_;

	};

}
