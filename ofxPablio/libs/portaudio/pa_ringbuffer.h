#ifndef RINGBUFFER_H
#define RINGBUFFER_H

/*
 * $Id: pa_ringbuffer.h 1549 2010-10-24 10:21:35Z rossb $
 * Portable Audio I/O Library
 * Ring Buffer utility.
 *
 * Author: Phil Burk, http://www.softsynth.com
 * modified for SMP safety on OS X by Bjorn Roche.
 * also allowed for const where possible.
 * modified for multiple-byte-sized data elements by Sven Fischer
 * 
 * memory allocation changed a bit and some things tweaked
 * to better fit openFrameworks by Nicolas d'Alessandro.
 *
 * Note that this is safe only for a single-thread reader
 * and a single-thread writer.
 *
 * This program is distributed with the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
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

/** @file
 @ingroup common_src
 @brief Single-reader single-writer lock-free ring buffer
 
 PaUtilRingBuffer is a ring buffer used to transport samples between
 different execution contexts (threads, OS callbacks, interrupt handlers)
 without requiring the use of any locks. This only works when there is
 a single reader and a single writer (ie. one thread or callback writes
 to the ring buffer, another thread or callback reads from it).
 
 The PaUtilRingBuffer structure manages a ring buffer containing N 
 elements, where N must be a power of two. An element may be any size 
 (specified in bytes).
 
 The memory area used to store the buffer elements must be allocated by 
 the client prior to calling PaUtil_InitializeRingBuffer() and must outlive
 the use of the ring buffer.
 */

#include <stdlib.h>
#include "pa_memorybarrier.h"

#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct _RingBuffer {

	long nOfItems; // # of items in the buffer
	long sizeOfItem; // # of bytes in one item
	
	long writeIndex; // index of the next writable element
	long readIndex; // index of the next readable element
	
	long bigMask; // used for wrapping indices with extra bit
	long smallMask; // used for fitting incides to buffer
	
	char *data; // pointer to actual data
	
} RingBuffer;

// --- initialization ---
unsigned long roundUpToNextPowerOfTwo( unsigned long n );
RingBuffer *initRingBuffer( long nOfItems, long sizeOfItem );

// --- flush and free ---
void flushRingBuffer( RingBuffer *rb );	
void freeRingBuffer( RingBuffer *rb );
	
// --- get read and write available ---
long getRingBufferReadAvailable( RingBuffer *rb );
long getRingBufferWriteAvailable( RingBuffer *rb );
	
// --- hidden writing manipulations ---
long getRingBufferWriteRegions( RingBuffer *rb, long nOfItems,
 void **dataPtr1, long *sizePtr1, void **dataPtr2, long *sizePtr2 );
long advanceRingBufferWriteIndex( RingBuffer *rb, long nOfItems );
	
// --- hidden reading manipulations ---
long getRingBufferReadRegions( RingBuffer *rb, long nOfItems,
 void **dataPtr1, long *sizePtr1, void **dataPtr2, long *sizePtr2 );
long advanceRingBufferReadIndex( RingBuffer *rb, long nOfItems );
	
// --- exposed read/write operations ---
long writeRingBuffer( RingBuffer *rb, const void *data, long nOfItems );
long readRingBuffer( RingBuffer *rb, void *data, long nOfItems );
	
#ifdef __cplusplus
}
#endif

#endif /* RINGBUFFER_H */
