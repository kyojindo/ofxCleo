#include "ofxAudioIn.h"

ofxAudioIn::ofxAudioIn( void ) {
	
	Type = ofxObjectAudioIn;
}

ofxAudioIn::~ofxAudioIn( void ) {
	
	if( InterlacedBuffer != NULL ) free( InterlacedBuffer );
}

void ofxAudioIn::setStream( ofxFablioStream *stream ) {
	
	Stream = stream;
}

void ofxAudioIn::setup( void ) {
	
	OutBuf.resize( Stream->nOfInputChannels );
	InterlacedBuffer = (float *) malloc( Stream->nOfInputChannels*tickBufferSize*sizeof(float) );
	
	for( int k=0; k<Stream->nOfInputChannels; k++ ) {
		
		setAudioOutlet( &OutBuf[k], AdcOut+k );
	}
}

void ofxAudioIn::tick( void ) {
	
	// read necessary audio samples from ringbuffer
	Stream->readAudio( InterlacedBuffer, tickBufferSize );
	
	// browse over the # of channels
	for( int m=0; m<Stream->nOfInputChannels; m++ ) {
		
		// tune pointer to mth channel
		OutBuf[m] = aOutlet[ AdcOut+m ];
		
		// browse over the audio buffer
		for( int k=0; k<tickBufferSize; k++ ) {
			
			OutBuf[ m ][ k ] = InterlacedBuffer[ Stream->nOfInputChannels*k + m ];
			// and interlace everything into one big chunck
		}
	}
}
