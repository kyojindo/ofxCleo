#include "testApp.h"

void testApp::setup( void ) {

	ofBackground( 50, 50, 50 ); // set background color
	ofSetColor( 200, 200, 200 ); // set stroke/fill color
	ofEnableSmoothing(); // enable graphic anti-alaising
	
	// PABLIO setup: sample rate, channels, vector size
	ofxPablioSetup( SAMPLERATE, 1, 0, VECTORSIZE );
}


void testApp::update( void ) {
	
	// THREADED AUDIO SYNTHESIS: where is the callback ? we don't care !
	
	// here we do something completely insane, just for the sake of showing
	// that it's possible: we generate the audio inside the GL rendering loop
	// it consumes a hell of CPU of course, but it should not glitch :)
	
	// damped sinusoid: mapping pitch and decay to mouse X and Y
	float pitch = ofMap( ofGetMouseX(), 0, ofGetWidth(), 100, 4000, true );
	float decay = ofMap( ofGetMouseY(), 0, ofGetHeight(), 0.1, 0.001, true );
	
	double phase = 0.0; // init phase
	
	// clean previous samples in the buffer
	for( int k=0; k<BOINGLEN; k++ ) boing[k] = 0.0;
	
	// synthesize damped sinusoid
	for( int k=0; k<BOINGLEN; k++ ) {
		
		// equation of the damped sinusoid with pitch and decay
		boing[k] = 0.9 * exp( -decay*( (double)k ) ) * sin( phase );
		
		// incrementing and moduloing the phase
		phase += ( ((double)TWO_PI*pitch)/(double)SAMPLERATE );
		if( phase > TWO_PI ) phase -= (double)TWO_PI;
	}
	
	// stack the chunk in PABLIO stream
	PABLIO->writeAudio( boing, BOINGLEN );
	
	// --- YES YOU HEAR THE SOUND ONLY COMING FROM LEFT SPEAKER ---
	// these functions aim at staying more or less low-level, so no multichannel
	// writing here; if you want stereo you have to interlace the buffer yourself,
	// but guess what, this is done in ofxCleo, which is coming in a few hours :)
}


void testApp::draw( void ) {
	
	for( int k=1; k<BOINGLEN; k++ ) {
		
		// we plot the damped sinusoid in the main window
		ofLine( k-1, ofMap(boing[k-1], -1, 1, ofGetHeight(), 0, false),
		        k, ofMap(boing[k], -1, 1, ofGetHeight(), 0, false) );
	}
}


void testApp::keyPressed( int key ) {
	
	
}


void testApp::keyReleased ( int key ) {
	
	
}


void testApp::mouseMoved( int x, int y ) {
	
	
}


void testApp::mousePressed( int x, int y, int button ) {
	
	
}


void testApp::mouseReleased( int x, int y, int button ) {
	
	
}


void testApp::mouseDragged( int x, int y, int button ) {
	
	
}


void testApp::windowResized( int w, int h ) {
	
	
}


void testApp::dragEvent( ofDragInfo dragInfo ) { 
	
	
}


void testApp::gotMessage( ofMessage msg ) {
	
	
}
