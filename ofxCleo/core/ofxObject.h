#pragma once

#include <cstdio>
#include <vector>
#include <ctime>

#include "ofMain.h"

#include "MemoryBarrier.h"
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
