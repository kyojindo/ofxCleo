#include "ofxPablioApp.h"

void ofxPablioApp::exit( void ) {
	
	PABLIO->closeAudio();
	delete PABLIO;
}

#if( OF_VERSION < 7 )
void ofxPablioApp::audioReceived( float *input, int size, int chan ) {
#else
void ofxPablioApp::audioIn( float *input, int size, int chan ) {
#endif
	
	int nOfWrittenItems = writeRingBuffer( PABLIO->inQueue, input, chan*size );
	//if( nOfWrittenItems < (chan*size) ) printf("ofxPablio - callback could not write enough samples\n");
}
	
#if( OF_VERSION < 7 )
void ofxPablioApp::audioRequested( float *output, int size, int chan ) {
#else
void ofxPablioApp::audioOut( float *output, int size, int chan ) {
#endif			
	
	//printf( "audio_out_callback: %i %i | %i\n", size, chan, PABLIO->outQueue->nOfItems );
	int nOfReadItems = readRingBuffer( PABLIO->outQueue, output, chan*size );
	//if( nOfReadItems < (chan*size) ) printf("ofxPablio - callback could not read enough samples\n");
	for( int k=nOfReadItems; k<(chan*size); k++ ) output[k] = 0.0;
}
	
void ofxPablioApp::ofxPablioSetup( int sampleRate, int nOfOutputs, int nOfInputs, int ioBufferSize ) {

	// setup the PABLIO stream with transfered arguments
	PABLIO = new ofxPablioStream( sampleRate, ioBufferSize, nOfInputs, nOfOutputs );
	
	if( PABLIO->nOfOutputChannels > 0 || PABLIO->nOfInputChannels > 0 ) {
		
#if( OF_VERSION < 7 || TARGET_OS_IPHONE == 1 )
		ofSoundStreamSetup( PABLIO->nOfOutputChannels, PABLIO->nOfInputChannels,
		this, PABLIO->sampleRate, PABLIO->framePerBuffer, 4 );
#else
		SoundStream.setup( this, PABLIO->nOfOutputChannels, PABLIO->nOfInputChannels,
		PABLIO->sampleRate, PABLIO->framePerBuffer, 4 );
#endif
	}
}
