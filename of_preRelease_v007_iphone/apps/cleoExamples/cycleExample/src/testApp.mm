#include "testApp.h"

void testApp::setup( void ) {	
	
	ofBackground( 50, 50, 50 ); // set background color
	ofSetColor( 200, 200, 200 ); // set stroke/fill color
	ofEnableSmoothing(); // enable graphic anti-alaising
	
	ofRegisterTouchEvents( this ); // active multi-touch
	ofxiPhoneAlerts.addListener( this ); // activate alerts
	ofxAccelerometer.setup(); // activate accelerometer
	
	// set iPhone to landscape left orientation
	ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT );
	
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
	
	
}


void testApp::draw( void ) {
	
	
}


void testApp::exit( void ) {
	
	
}


void testApp::touchDown( ofTouchEventArgs &touch ) {
	
	
}


void testApp::touchMoved( ofTouchEventArgs &touch ) {
	
	// changing the inlet variables is really not thread-safe
	// but it seems to work fine; I have an idea though how to
	// make it thread-safe, but it will take a bit of work !
	
	Rand.Speed = ofMap( touch.x, 0, ofGetWidth(), 100, 10, true );
}


void testApp::touchUp( ofTouchEventArgs &touch ) {
	
	
}


void testApp::touchDoubleTap( ofTouchEventArgs &touch ) {
	
	
}


void testApp::touchCancelled( ofTouchEventArgs& args ) {
	
	
}


void testApp::gotFocus( void ) {
	
	
}


void testApp::lostFocus( void ) {
	
	
}


void testApp::deviceOrientationChanged( int newOrientation ) {
	
	
}


void testApp::gotMemoryWarning( void ) {
	
	
}
