#pragma once

#include "ofMain.h"
#include "ofxCleo.h"

#include "ofxObjects.h" // this contains the current objects

const int SAMPLERATE = 44100; // audio samplerate ( as in RtAudio )
const int VECTORSIZE = 512; // audio buffersize ( as in RtAudio )
const int TICKSIZE = 64; // ofxCleo synthesis vector size

// we use a specific app to encapsulate the patching
// aspects of ofxCleo so that users focus on what matters

class testApp : public ofxCleoApp {

  public:
	
	void setup( void );
	void update( void );
	void draw( void );

	void keyPressed( int key );
	void keyReleased( int key );
	
	void mouseMoved( int x, int y );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseDragged( int x, int y, int button );
	
	void windowResized( int w, int h );
	void dragEvent( ofDragInfo dragInfo );
	void gotMessage( ofMessage msg );
	
	ofxRand Rand; // a control-rate random number generator
	ofxMtof Mtof; // a MIDI note # to Hertz conversion
	ofxCycle Cycle; // a sinusoidal audio generator
};
