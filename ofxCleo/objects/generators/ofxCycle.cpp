#include "ofxCycle.h"

void ofxCycle::setup( void ) {
	
	setControlInlet( &Pitch, CyclePitch );
	setControlInlet( &Volume, CycleVolume );
	setAudioOutlet( &OutBuf, CycleSigOut );
	
	Pitch = 0.0;
	PitchInHz = 110.0;
	
	Volume = 0.8;
	phase = 0.0;
}

void ofxCycle::update( void ) {

	PitchInHz = 110.0 * powf( 2.0, (Pitch/12.0) );
}

void ofxCycle::tick( void ) {
	
	for( int k=0; k<tickBufferSize; k++ ) {
	
		OutBuf[k] = Volume * cos( phase );
		phase += ( ((double)TWO_PI*PitchInHz)/(double)sampleRate );
		if( phase > TWO_PI ) phase -= (double)TWO_PI;
	}
}
