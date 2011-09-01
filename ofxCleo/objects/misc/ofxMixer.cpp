#include "ofxMixer.h"

void ofxMixer::setup( void ) {
	
	setAudioOutlet( &OutBuf, MixerSigOut );
}

void ofxMixer::setNumOfInputs( int n ) {
	
	nOfInputs = n;
	InBuf.resize( nOfInputs );
	
	for( int k=0; k<nOfInputs; k++ ) {
		
		setAudioInlet( &InBuf[k], MixerSigIn+k );
	}
}

void ofxMixer::tick( void ) {
	
	for( int k=0; k<tickBufferSize; k++ ) {
		
		OutBuf[ k ] = 0.0;
		
		for( int m=0; m<nOfInputs; m++ ) {
			
			OutBuf[ k ] += InBuf[ m ][ k ];
		}
	}
}
