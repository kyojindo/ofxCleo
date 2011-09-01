#include "ofxAudioOut.h"

ofxAudioOut::ofxAudioOut( void ) {
	
	Type = ofxObjectAudioOut;
}

ofxAudioOut::~ofxAudioOut( void ) {
	
	if( InterlacedBuffer != NULL ) free( InterlacedBuffer );
}

void ofxAudioOut::setStream( ofxFablioStream *stream ) {
	
	Stream = stream;
}

void ofxAudioOut::setup( void ) {
	
	InBuf.resize( Stream->nOfOutputChannels );
	InterlacedBuffer = (float *) malloc( Stream->nOfOutputChannels*tickBufferSize*sizeof(float) );
	
	for( int k=0; k<Stream->nOfOutputChannels; k++ ) {
		
		setAudioInlet( &InBuf[k], DacIn+k );
	}
}

void ofxAudioOut::tick( void ) {
	
	// browse over the # of channels
	for( int m=0; m<Stream->nOfOutputChannels; m++ ) {
		
		// tune pointer to mth channel
		InBuf[m] = *( aInlet[ DacIn+m ] );
			
		// browse over the audio buffer
		for( int k=0; k<tickBufferSize; k++ ) {
			
			InterlacedBuffer[ Stream->nOfOutputChannels*k + m ] = InBuf[ m ][ k ];
			// and interlace everything into one big chunck
		}
	}
	
	// write the resulting audio samples into ringbuffer
	Stream->writeAudio( InterlacedBuffer, tickBufferSize );
}
