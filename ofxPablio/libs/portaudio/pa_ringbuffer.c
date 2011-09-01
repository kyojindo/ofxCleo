#include "pa_ringbuffer.h"

/*
 * $Id: pa_ringbuffer.c 1574 2011-02-01 11:18:16Z rossb $
 * Portable Audio I/O Library
 * Ring Buffer utility.
 *
 * Author: Phil Burk, http://www.softsynth.com
 * modified for SMP safety on Mac OS X by Bjorn Roche
 * modified for SMP safety on Linux by Leland Lucius
 * also, allowed for const where possible
 * modified for multiple-byte-sized data elements by Sven Fischer
 *
 * memory allocation changed a bit and some things tweaked
 * to better fit openFrameworks by Nicolas d'Alessandro.
 *
 * Note that this is safe only for a single-thread reader and a
 * single-thread writer.
 *
 * This program uses the PortAudio Portable Audio Library.
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

/**
 @file
 @ingroup common_src
 */

// utility for rounding size to next power of two
unsigned long roundUpToNextPowerOfTwo( unsigned long n ) {
	
	long nOfBits = 0;
	
    if( ((n-1) & n) == 0 ) return n;
	
    while( n > 0 ) {
		
        n = n>>1;
        nOfBits++;
    }
	
    return ( 1<<nOfBits );
}

// initialization of the ringbuffer
RingBuffer *initRingBuffer( long nOfItems, long sizeOfItem ) {
	
	// allocate the structure of the ringbuffer
	RingBuffer *rb = (RingBuffer *) malloc( sizeof(RingBuffer) );
	
	// round up to next power of two
	nOfItems = roundUpToNextPowerOfTwo( nOfItems );
	
	// allocate the memory for the ringbuffer
	rb->data = (char *) malloc( nOfItems*sizeOfItem );
	
	rb->sizeOfItem = sizeOfItem;
	rb->nOfItems = nOfItems;
	
	rb->bigMask = (nOfItems*2)-1;
	rb->smallMask = nOfItems-1;
	
	flushRingBuffer( rb );
	
	return rb;
}

// flushing of the ringbuffer
void flushRingBuffer( RingBuffer *rb ) {
	
	rb->writeIndex = 0;
	rb->readIndex = 0;
}

// free memory of the ringbuffer
void freeRingBuffer( RingBuffer *rb ) {
	
	if( rb->data != NULL )free( rb->data );
	if( rb != NULL ) free( rb );
}

// get number of slots available for reading
long getRingBufferReadAvailable( RingBuffer *rb ) {
	
	ReadMemoryBarrier();
	return ( (rb->writeIndex - rb->readIndex) & rb->bigMask );
}

// get number of slots available for writing
long getRingBufferWriteAvailable( RingBuffer *rb ) {
	
	return ( rb->nOfItems - getRingBufferReadAvailable( rb ) );
}

// get pointers to regions available for writing
long getRingBufferWriteRegions( RingBuffer *rb, long nOfItems,
 void **dataPtr1, long *sizePtr1, void **dataPtr2, long *sizePtr2 ) {

    long available = getRingBufferWriteAvailable( rb );	
    if( nOfItems > available ) nOfItems = available;
    long index = rb->writeIndex & rb->smallMask;
	
	// check to see if write is not contiguous
    if( ( index + nOfItems ) > rb->nOfItems ) {
		
        long firstHalf = rb->nOfItems - index;
		
        *dataPtr1 = &rb->data[ index*rb->sizeOfItem ];
        *sizePtr1 = firstHalf;
		
        *dataPtr2 = &rb->data[ 0 ];
        *sizePtr2 = nOfItems - firstHalf;
		
    } else {
		
        *dataPtr1 = &rb->data[ index*rb->sizeOfItem ];
        *sizePtr1 = nOfItems;
		
        *dataPtr2 = NULL;
        *sizePtr2 = 0;
    }
	
    return nOfItems;
}

// advance ringbuffer writing position
long advanceRingBufferWriteIndex( RingBuffer *rb, long nOfItems ) {
	
	WriteMemoryBarrier();
	return rb->writeIndex = ( rb->writeIndex + nOfItems ) & rb->bigMask;
}

// get pointers to regions available for reading
long getRingBufferReadRegions( RingBuffer *rb, long nOfItems,
 void **dataPtr1, long *sizePtr1, void **dataPtr2, long *sizePtr2 ) {

    long available = getRingBufferReadAvailable( rb );	
    if( nOfItems > available ) nOfItems = available;
    long index = rb->readIndex & rb->smallMask;
	
	// check to see if read is not contiguous
    if( ( index + nOfItems ) > rb->nOfItems ) {
		
        long firstHalf = rb->nOfItems - index;
		
        *dataPtr1 = &rb->data[ index*rb->sizeOfItem ];
        *sizePtr1 = firstHalf;
		
        *dataPtr2 = &rb->data[ 0 ];
        *sizePtr2 = nOfItems - firstHalf;
		
    } else {
		
        *dataPtr1 = &rb->data[ index*rb->sizeOfItem ];
        *sizePtr1 = nOfItems;
		
        *dataPtr2 = NULL;
        *sizePtr2 = 0;
    }
	
    return nOfItems;
}

// advance ringbuffer reading position
long advanceRingBufferReadIndex( RingBuffer *rb, long nOfItems ) {
	
	WriteMemoryBarrier();
	return rb->readIndex = ( rb->readIndex + nOfItems ) & rb->bigMask;
}

// exposed call for writing in the ringbuffer
long writeRingBuffer( RingBuffer *rb, const void *data, long nOfItems ) {
	
	long size1, size2, nOfWrittenItems;
	void *data1, *data2;
	
    nOfWrittenItems = getRingBufferWriteRegions( rb,
	 nOfItems, &data1, &size1, &data2, &size2 );
	
    if( size2 > 0 ) {
		
        memcpy( data1, data, size1*rb->sizeOfItem );
        data = ((char *)data) + size1*rb->sizeOfItem;
        memcpy( data2, data, size2*rb->sizeOfItem );
		
    } else {
		
        memcpy( data1, data, size1*rb->sizeOfItem );
    }
	
    advanceRingBufferWriteIndex( rb, nOfWrittenItems );
	
    return nOfWrittenItems;
}

// exposed call for reading in the ringbuffer
long readRingBuffer( RingBuffer *rb, void *data, long nOfItems ) {
	
	long size1, size2, nOfReadItems;
	void *data1, *data2;
	
    nOfReadItems = getRingBufferReadRegions( rb,
	 nOfItems, &data1, &size1, &data2, &size2 );
	
    if( size2 > 0 ) {
		
        memcpy( data, data1, size1*rb->sizeOfItem );
        data = ((char *)data) + size1*rb->sizeOfItem;
        memcpy( data, data2, size2*rb->sizeOfItem );
		
    } else {
		
        memcpy( data, data1, size1*rb->sizeOfItem );
    }
	
    advanceRingBufferReadIndex( rb, nOfReadItems );
	
    return nOfReadItems;
}
