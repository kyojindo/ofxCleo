/*
 * ofxPablioApp.cpp ~ openFrameworks
 * Nicolas d'Alessandro <nicolas@dalessandro.be>
 * and Johnty Wang <info@johnty.ca>
 *
 * ofxPablio is an adaption of Bencina & Burk's PABLIO
 * library for asynchronous read/write of audio streams.
 * It allows to manipulate audio samples outside usual
 * oF I/O callbacks through lock-free ringbuffers.
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
