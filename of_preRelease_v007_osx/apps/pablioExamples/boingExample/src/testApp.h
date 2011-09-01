#pragma once

#include "ofMain.h"
#include "ofxPablio.h"

const int BOINGLEN = 4096; // length of the local buffer
const int SAMPLERATE = 44100; // audio samplerate ( as in RtAudio )
const int VECTORSIZE = 512; // audio buffersize ( as in RtAudio )

// we use a specific app to encapsulate the streaming
// aspect of PABLIO so that users focus on what matters

class testApp : public ofxPablioApp {

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
	
	float boing[BOINGLEN]; // boing !
};
