#include "testApp.h"

void testApp::setup( void ) {

	ofBackground( 50, 50, 50 ); // set background color
	ofSetColor( 200, 200, 200 ); // set stroke/fill color
	ofEnableSmoothing(); // enable graphic anti-alaising
	
	// ofxCleo setup: samplerate, channels, vector & tick sizes
	ofxCleoSetup( SAMPLERATE, 2, 0, VECTORSIZE, TICKSIZE );
	
	// ofxCleo allows to "patch" inlets and outlets from objets you
	// declared in the testApp class; this patchcord will take care
	// of synchronizing I/O and audio streams in the background, in
	// other to solve the graph that you have been drawing here.
	
	ofxPatch( Rand, RandOut, Mtof, MtofIn ); // patch random output in MIDI->Hz
	ofxPatch( Mtof, MtofOut, Cycle, CyclePitch ); // patch result in Hz in cycle pitch
	
	ofxPatch( Cycle, CycleSigOut, DAC, DacSigLeft ); // patch cycle audio out in DAC (L)
	ofxPatch( Cycle, CycleSigOut, DAC, DacSigRight ); // patch cycle audio out in DAC (R)
	
	// variables corresponding to inlets can also be directly accessed
	// and modified manually; if they are patched, they'll be overwritten
	
	Rand.Min = 60; // minimum bound of the random ( here taken as MIDI note )
	Rand.Max = 90; // maximum bound of the random ( here taken as MIDI note )
	Rand.Speed = 100; // refreshing rate of the random in milliseconds
	
	Cycle.Amplitude = 0.9; // we just patch the pitch inlet so other
	Cycle.Phase = 0.0; // inlets need to be adjusted by hand
	
	ofxCleoStart(); // start the ofxCleo synthesis
}


void testApp::update( void ) {
	
	// changing the inlet variables is really not thread-safe
	// but it seems to work fine; I have an idea though how to
	// make it thread-safe, but it will take a bit of work !
	
	Rand.Speed = ofMap( ofGetMouseX(), 0, ofGetWidth(), 100, 10, true );
}


void testApp::draw( void ) {
	
	
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
