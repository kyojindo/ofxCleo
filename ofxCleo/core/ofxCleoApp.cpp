/*
 * ofxCleoApp.cpp ~ openFrameworks
 * Nicolas d'Alessandro <nicolas@dalessandro.be>
 * and Johnty Wang <info@johnty.ca>
 *
 * ofxCleo is an audio patching utiliy for openFrameworks.
 * It allows to connect inlets and outlets from various kinds
 * of objects ( controllers, modifiers, generators, filters )
 * into a graph and solve it in realtime. ofxCleo also allows
 * to simply create new objects to be patched by inheritance.
 *
 *
 * MIT License ~ Copyright (c) 2011 University of British Columbia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ofxCleoApp.h"

ofxCleoApp::ofxCleoApp( void ) {

	
}

void ofxCleoApp::exit( void ) {
	
	PABLIO->closeAudio(); // close the audio stream
	Scheduler.waitForThread( true ); // stop scheduler
	
	// free no-inlet buffer contained in patcher
	if( Patcher.aNoInlet != NULL ) free( Patcher.aNoInlet );
	delete PABLIO; // delete stream
}

#if( OF_VERSION < 7 )
void ofxCleoApp::audioReceived( float *input, int size, int chan ) {
#else
void ofxCleoApp::audioIn( float *input, int size, int chan ) {
#endif
	
	int nOfWrittenItems = writeRingBuffer( PABLIO->inQueue, input, chan*size );
	//if( nOfWrittenItems < (chan*size) ) printf("ofxFablio - callback could not write enough samples\n");
}

#if( OF_VERSION < 7 )
void ofxCleoApp::audioRequested( float *output, int size, int chan ) {
#else
void ofxCleoApp::audioOut( float *output, int size, int chan ) {
#endif
	
	int nOfReadItems = readRingBuffer( PABLIO->outQueue, output, chan*size );
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
	
	// setup the PABLIO stream with transfered arguments
	PABLIO = new ofxPablioStream( sampleRate, ioBufferSize, nOfInputs, nOfOutputs );
	
	// decide if scheduler has to run audio routines based on i/o
	if( nOfInputs > 0 || nOfOutputs > 0 ) Scheduler.setAudio( true );
	
	// link stream to DAC and ADC objets
	DAC.setStream( PABLIO ); ADC.setStream( PABLIO );
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
