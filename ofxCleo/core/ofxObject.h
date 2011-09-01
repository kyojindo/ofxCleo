/*
 * ofxObject.h ~ openFrameworks
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

#include <cstdio>
#include <vector>
#include <ctime>

#include "ofMain.h"

#include "pa_memorybarrier.h"

#if( OF_VERSION < 7 )
#include "ofxThread.h"
#else
#include "ofThread.h"
#endif

// -- custom types --
typedef int ofxObjectField;
typedef short ofxObjectType;
typedef short ofxIoletType;

#if( OF_VERSION < 7 )
typedef ofxThread ofThread;
#endif

// -- inlet/outlet types --
const ofxIoletType ofxIoletBang = 0;
const ofxIoletType ofxIoletControl = 1;
const ofxIoletType ofxIoletAudio = 2;

// -- object types --
const ofxObjectType ofxObjectParent = 0;
const ofxObjectType ofxObjectThreaded = 1;
const ofxObjectType ofxObjectAudioOut = 2;
const ofxObjectType ofxObjectAudioIn = 3;

using namespace std;

class ofxObjectThread;

class ofxObject {
	
  public:
	
	ofxObject( void );
	virtual ~ofxObject( void );
	
	// -- graph --
	void resetToken( void );
	void incrementToken( void );
	void emitToken( void );
	
	// -- map control-rate inlets/outlets --
	void setControlInlet( double *Variable, ofxObjectField Field );
	void setControlOutlet( double *Variable, ofxObjectField Field );
	
	// -- map audio-rate inlets/outlets --
	void setAudioInlet( float **Buffer, ofxObjectField Field );
	void setAudioOutlet( float **Buffer, ofxObjectField Field );
	
	// -- set threading options --
	void setAsThreaded( void );
	
	// -- setup functions --
	virtual void setup( void );
	virtual void _setup( void );
	
	// -- control functions --
	virtual void update( void );
	void _update( void );
	
	// -- audio functions --
	virtual void tick( void );
	void _tick( void );
	
	// -- patcher --
	ofxObjectType Type;
	int Id, nOfTargets, TokenCount;
	vector<ofxObject *> Target;
	bool isAttached;
	
	// -- scheduler --
	bool isInControlScheduler;
	bool isInAudioScheduler;
	
	// -- threading --
	ofxObjectThread *ObjectThread;
	bool isThreaded;
	
	// -- i/o type masks --
	vector<ofxIoletType> InletType;
	vector<ofxIoletType> OutletType;
	
	// -- control inlets --
	vector<bool> kInletPatched;
	vector<double *> kInletLookup;
	vector<bool> kInletThreaded;
	vector<double *> kInlet;
	
	// -- inlet mapping --
	vector<float> kInletMultiplier;
	vector<float> kInletOffset;
	
	// -- control outlets --
	vector<double> kOutletBackup;
	vector<bool> kOutletThreaded;
	vector<double *> kOutlet;
	
	// -- audio in/outs --
	vector<float **> aInlet;
	vector<float *> aOutlet;
	
	// -- audio parameters --
	int sampleRate;
	int tickBufferSize;
	int ioBufferSize;
	float *aNoInlet;
	
  protected:
	
	// -- resize in/out buffers --
	void resizeInletBuffers( int Length );
	void resizeOutletBuffers( int Length );
};

class ofxObjectThread : public ofThread {
	
public:
	
	ofxObjectThread( ofxObject *ObjectPtr );
	virtual ~ofxObjectThread( void );
	
	void threadedFunction( void );
	
	ofxObject *Object;
};
