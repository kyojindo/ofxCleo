/*
 * ofxScheduler.cpp ~ openFrameworks
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

#include "ofxScheduler.h"

ofxScheduler::ofxScheduler( void ) {
	
	isPaused = false;
}

ofxScheduler::~ofxScheduler( void ) {
	
	
}

void ofxScheduler::setAudio( bool Mode ) {
	
	isAudioOn = Mode;
}

void ofxScheduler::setPatcher( ofxPatcher *PatcherPtr ) {
	
	Patcher = PatcherPtr;
}

void ofxScheduler::threadedFunction( void ) {
	
	while( threadRunning ) {

		if( !isPaused ) tick();
		if( !isAudioOn ) usleep( 10000 );
	}
}

void ofxScheduler::pause( bool Mode ) {
	
	isPaused = Mode;
}

void ofxScheduler::tick( void ) {

	// propage control and audio processing the the graph
	for( int k=0; k<Patcher->SortedObjectList.size(); k++ ) {
		
		if( Patcher->SortedObjectList[k]->isInControlScheduler ) Patcher->SortedObjectList[k]->_update();
		if( Patcher->SortedObjectList[k]->isInAudioScheduler && isAudioOn ) Patcher->SortedObjectList[k]->_tick();
	}
}
