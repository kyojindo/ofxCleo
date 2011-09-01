#include "ofxRand.h"

void ofxRand::setup( void ) {
		
	setAsThreaded();
	
	setControlOutlet( &Res, RandOut );
	setControlInlet( &Speed, RandSpeed );
	setControlInlet( &Min, RandMin );
	setControlInlet( &Max, RandMax );
	
	Min = 0.0; Max = 1.0;
	Speed = 10.0;
}

void ofxRand::update( void ) {
	
	Res = ofRandom( Min, Max );
	usleep( 1000*Speed );
}