/*
 * ofxCleoApp.h ~ openFrameworks
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

#pragma once

#include "ofMain.h"
#include "ofxPatcher.h"
#include "ofxScheduler.h"
#include "ofxAudioOut.h"
#include "ofxAudioIn.h"
#include "ofxPablio.h"

#if( TARGET_OS_IPHONE == 1 )
#include "ofxiPhone.h"
#endif

#if( TARGET_OS_IPHONE == 1 )
class ofxCleoApp : public ofxiPhoneApp {
#else
class ofxCleoApp : public ofBaseApp {
#endif
	
  public:
	
	ofxCleoApp( void );
	
	virtual void setup( void ) {}
	virtual void update( void ) {}
	virtual void draw( void ) {}
	void exit( void );
	
#if( TARGET_OS_IPHONE == 1 )
	virtual void touchUp( ofTouchEventArgs &touch ) {}
	virtual void touchDown( ofTouchEventArgs &touch ) {}
	virtual void touchDoubleTap( ofTouchEventArgs &touch ) {}
	virtual void touchMoved( ofTouchEventArgs &touch ) {}
	
	virtual void gotMemoryWarning( void ) {}
	virtual void deviceOrientationChanged( int newOrientation ) {}
	virtual void lostFocus( void ) {}
	virtual void gotFocus( void ) {}
#else
	virtual void keyPressed( int key ) {}
	virtual void keyReleased( int key ) {}
	
	virtual void mousePressed( int x, int y, int button ) {}
	virtual void mouseReleased( int x, int y, int button ) {}
	virtual void mouseDragged( int x, int y, int button ) {}
	virtual void mouseMoved( int x, int y ) {}
	
	virtual void windowResized( int w, int h ) {}
#endif
	
#if( OF_VERSION < 7 )
	void audioReceived( float *input, int size, int chan );
	void audioRequested( float *output, int size, int chan );
#else
	void audioOut( float *input, int size, int chan );
	void audioIn( float *output, int size, int chan );
#endif
	
	void ofxCleoSetup( int sampleRate, int nOfOutputs, int nOfInputs, int ioBufferSize, int tickBufferSize );
	
	void ofxPatch( ofxObject &Source, ofxObjectField SourceField, ofxObject &Target, ofxObjectField TargetField );
	void ofxPatch( ofxObject &Source, ofxObjectField SourceField, ofxObject &Target, ofxObjectField TargetField,
	 double SourceLowBound, double SourceHighBound, double TargetLowBound, double TargetHighBound );
	
	void ofxAttach( ofxObject &Object );
	void ofxCleoStart( void );
	
  protected:
	
#if( OF_VERSION >= 7 || TARGET_OS_IPHONE != 1 )
	ofSoundStream SoundStream;
#endif
	
	ofxPatcher Patcher; 
	ofxScheduler Scheduler;
	ofxPablioStream *PABLIO;
	ofxAudioOut DAC;
	ofxAudioIn ADC;
};
