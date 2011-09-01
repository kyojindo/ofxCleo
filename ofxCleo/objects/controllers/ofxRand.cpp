/*
 * ofxRand.cpp ~ openFrameworks
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

#include "ofxRand.h"

void ofxRand::setup( void ) {
		
	setAsThreaded();
	
	setControlOutlet( &Res, RandOut );
	setControlInlet( &Speed, RandSpeed );
	setControlInlet( &Min, RandMin );
	setControlInlet( &Max, RandMax );
	
	Min = 0.0; Max = 1.0;
	Speed = 10.0;
}

void ofxRand::update( void ) {
	
	Res = ofRandom( Min, Max );
	usleep( 1000*Speed );
}