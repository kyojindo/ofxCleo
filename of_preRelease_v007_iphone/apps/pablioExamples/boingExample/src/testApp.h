#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxPablio.h"

const int BOINGLEN = 4096; // length of the local buffer
const int SAMPLERATE = 44100; // audio samplerate ( as in OpenAL )
const int VECTORSIZE = 512; // audio buffersize ( as in OpenAL )

// we use a specific app to encapsulate the streaming
// aspect of PABLIO so that users focus on what matters

class testApp : public ofxPablioApp {
	
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
	
	float boing[BOINGLEN]; // boing !
	float x, y; // finger position
};
