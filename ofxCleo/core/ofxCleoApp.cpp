#include "ofxCleoApp.h"

ofxCleoApp::ofxCleoApp( void ) {

	
}

void ofxCleoApp::exit( void ) {
	
	Stream->closeAudio(); // close the audio stream
	Scheduler.waitForThread( true ); // stop scheduler
	
	// free no-inlet buffer contained in patcher
	if( Patcher.aNoInlet != NULL ) free( Patcher.aNoInlet );
	delete Stream; // delete stream
}

#if( OF_VERSION < 7 )
void ofxCleoApp::audioReceived( float *input, int size, int chan ) {
#else
void ofxCleoApp::audioIn( float *input, int size, int chan ) {
#endif
	
	int nOfWrittenItems = writeRingBuffer( Stream->inQueue, input, chan*size );
	//if( nOfWrittenItems < (chan*size) ) printf("ofxFablio - callback could not write enough samples\n");
}

#if( OF_VERSION < 7 )
void ofxCleoApp::audioRequested( float *output, int size, int chan ) {
#else
void ofxCleoApp::audioOut( float *output, int size, int chan ) {
#endif
	
	int nOfReadItems = readRingBuffer( Stream->outQueue, output, chan*size );
	//if( nOfReadItems < (chan*size) ) printf("ofxFablio - callback could not read enough samples\n");
	for( int k=nOfReadItems; k<(chan*size); k++ ) output[k] = 0.0;
}

void ofxCleoApp::ofxCleoSetup( int sampleRate, int nOfOutputs, int nOfInputs, int ioBufferSize, int tickBufferSize ) {
	
	// update patcher parameters
	Patcher.sampleRate = sampleRate;
	Patcher.tickBufferSize = tickBufferSize;
	Patcher.ioBufferSize = ioBufferSize;
	
	// creation of a zero buffer in case we unplug audio i/o
	Patcher.aNoInlet = (float *) malloc( tickBufferSize*sizeof(float) );
	for( int k=0; k<tickBufferSize; k++ ) Patcher.aNoInlet[k] = 0.0;
	
	// setup the FABLIO stream with transfered arguments
	Stream = new ofxFablioStream( sampleRate, ioBufferSize, nOfInputs, nOfOutputs );
	
	// decide if scheduler has to run audio routines based on i/o
	if( nOfInputs > 0 || nOfOutputs > 0 ) Scheduler.setAudio( true );
	
	// link stream to DAC and ADC objets
	DAC.setStream( Stream ); ADC.setStream( Stream );
}

void ofxCleoApp::ofxPatch( ofxObject &Source, ofxObjectField SourceField, ofxObject &Target, ofxObjectField TargetField,
 double SourceLowBound, double SourceHighBound, double TargetLowBound, double TargetHighBound ) {
	
	Patcher.patch( Source, SourceField, Target, TargetField,
	 SourceLowBound, SourceHighBound, TargetLowBound, TargetHighBound );
}

void ofxCleoApp::ofxPatch( ofxObject &Source, ofxObjectField SourceField, ofxObject &Target, ofxObjectField TargetField ) {
	
	Patcher.patch( Source, SourceField, Target, TargetField, 0.0, 1.0, 0.0, 1.0 );
}

void ofxCleoApp::ofxAttach( ofxObject &Object ) {
	
	Patcher.attach( Object );
}

void ofxCleoApp::ofxCleoStart( void ) {
	
	Scheduler.setPatcher( &Patcher );
	Scheduler.startThread( false, false );
	
	if( Stream->nOfOutputChannels > 0 || Stream->nOfInputChannels > 0 ) {
		
#if( OF_VERSION < 7 || TARGET_OS_IPHONE == 1 )
		ofSoundStreamSetup( Stream->nOfOutputChannels, Stream->nOfInputChannels,
		this, Stream->sampleRate, Stream->framePerBuffer, 4 );
#else
		SoundStream.setup( this, Stream->nOfInputChannels, Stream->nOfOutputChannels,
		Stream->sampleRate, Stream->framePerBuffer, 4 );
#endif
	}
}
