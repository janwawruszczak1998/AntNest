#include "NC2DArray.hpp"

NC2DArray::NC2DArray ( int width, int height ) 
: array(width) {
	for (int i = 0; i < width; ++i){
		array[i].resize(height, ' ');
    }
}

NC2DArray::~NC2DArray () { };

char NC2DArray::at( int x, int y ) const {
	return array[y][x];
}


void NC2DArray::set ( int x, int y, char value ) {
	array[y][x] = value;
}

int NC2DArray::width() const {
	return array.size();
}


int NC2DArray::height() const {
	return array[0].size();
}