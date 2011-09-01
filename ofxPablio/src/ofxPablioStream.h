/*
 * $Id: pablio.h 1083 2006-08-23 07:30:49Z rossb $
 * PABLIO.h
 * Portable Audio Blocking read/write utility.
 *
 * Author: Phil Burk, http://www.softsynth.com/portaudio/
 *
 * Include file for PABLIO, the Portable Audio Blocking I/O Library.
 * PABLIO is built on top of PortAudio, the Portable Audio Library.
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

#pragma once

#include <ctime>
#include "pa_ringbuffer.h"

class ofxPablioStream {
	
  public:
	
	ofxPablioStream( long sr, long bufsize, long inchan, long outchan );
	virtual ~ofxPablioStream( void ); // create and destroy the stream
	
	void closeAudio( void ); // close the PABLIO stream
	long writeAudio( float *data, long size ); // write samples in the PABLIO stream
	long readAudio( float *data, long size ); // read samples from the PABLIO stream
	
	bool isRunning;
	
	RingBuffer *inQueue;
	RingBuffer *outQueue;
	
	long nOfInputChannels;
	long nOfOutputChannels;
	long framePerBuffer;
	long sampleRate;
};
