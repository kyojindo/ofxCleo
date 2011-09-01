#include "ofxPablioStream.h"

/*
 * $Id: pablio.c 1151 2006-11-29 02:11:16Z leland_lucius $
 * pablio.c
 * Portable Audio Blocking Input/Output utility.
 *
 * Author: Phil Burk, http://www.softsynth.com
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * This file has been converted into a C++ class in order to fit its
 * inclusion into openFrameworks, by Nicolas d'Alessandro.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however, 
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also 
 * requested that these non-binding requests be included along with the 
 * license above.
 */

// create the PABLIO stream: initialize I/O ringbuffers
ofxPablioStream::ofxPablioStream( long sr, long bufsize, long inchan, long outchan ) {
	
	sampleRate = sr;
	isRunning = true;
	
	nOfInputChannels = inchan;
	nOfOutputChannels = outchan;
	framePerBuffer = bufsize;
	
	if( nOfInputChannels > 0 ) inQueue = initRingBuffer( 8*nOfInputChannels*framePerBuffer, sizeof(float) );
	if( nOfOutputChannels > 0 ) outQueue = initRingBuffer( 8*nOfOutputChannels*framePerBuffer, sizeof(float) );
	
	// make write FIFO appear full initially
	if( nOfOutputChannels > 0 ) {
		long nOfItems = getRingBufferWriteAvailable( outQueue );
		advanceRingBufferWriteIndex( outQueue, nOfItems );
	}
}

// destroy the PABLIO stream
ofxPablioStream::~ofxPablioStream( void ) {
	
	if( nOfOutputChannels > 0 ) freeRingBuffer( outQueue );
	if( nOfInputChannels > 0 ) freeRingBuffer( inQueue );
}

// close the PABLIO stream
void ofxPablioStream::closeAudio( void ) {
	
	isRunning = false;
	if( nOfOutputChannels > 0 ) flushRingBuffer( outQueue );
	if( nOfInputChannels > 0 ) flushRingBuffer( inQueue );
}

 // write samples in the PABLIO stream
long ofxPablioStream::writeAudio( float *data, long size ) {
	
	long nOfWrittenItems;
	char *ptr = (char *)data;
	
	long nOfItems = nOfOutputChannels*size;
	
	while( nOfItems > 0 && isRunning ) {

		nOfWrittenItems = writeRingBuffer( outQueue, ptr, nOfItems );
		nOfItems -= nOfWrittenItems; ptr += (nOfWrittenItems*sizeof(float));
		if( nOfItems > 0 ) usleep( 10000 );
	}
	
	return size;
}

 // read samples from the PABLIO stream
long ofxPablioStream::readAudio( float *data, long size ) {
	
	long nOfReadItems;
	char *ptr = (char *)data;
	
	long nOfItems = nOfInputChannels*size;
	
	while( nOfItems > 0 && isRunning ) {
		
		nOfReadItems = readRingBuffer( inQueue, ptr, nOfItems );
		nOfItems -= nOfReadItems; ptr += (nOfReadItems*sizeof(float));
		if( nOfItems > 0 ) usleep( 10000 );
	}
	
	return size;
}
