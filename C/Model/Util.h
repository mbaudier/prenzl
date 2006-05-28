#pragma once

//! saturates to 0 and maxValue
inline int quickModulo( int value, int maxValue) {
	if( value < 0 ) {
		value += maxValue;
	}
	else if( value >= maxValue ) {
		value -= maxValue;
	}
	return value;
}

inline int charSaturate(int value) {
	if(value < 0) {
		value = 0;
	}
	if( value > 255) {
		value = 255;
	}
	return value;
}

inline int saturate(int value, int min, int max) {
	if(value < min) {
		value = min;
	}
	if(value > max) {
		value = max;
	}
	return value;
}
