#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxCleo.h"

#include "ofxObjects.h" // this contains the current objects

const int SAMPLERATE = 44100; // audio samplerate ( as in OpenAL )
const int VECTORSIZE = 512; // audio buffersize ( as in OpenAL )
const int TICKSIZE = 64; // ofxCleo synthesis vector size

// we use a specific app to encapsulate the patching
// aspects of ofxCleo so that users focus on what matters

class testApp : public ofxCleoApp {
	
  public:
	
	void setup( void );
	void update( void );
	void draw( void );
	void exit( void );
	
	void touchDown( ofTouchEventArgs &touch );
	void touchMoved( ofTouchEventArgs &touch );
	void touchUp( ofTouchEventArgs &touch );
	
	void touchDoubleTap( ofTouchEventArgs &touch );
	void touchCancelled( ofTouchEventArgs &touch );

	void gotFocus( void );
	void lostFocus( void );
	
	void deviceOrientationChanged( int newOrientation );
	void gotMemoryWarning( void );
	
	ofxRand Rand; // a control-rate random number generator
	ofxMtof Mtof; // a MIDI note # to Hertz conversion
	ofxCycle Cycle; // a sinusoidal audio generator
};
