#pragma once

#include "ofMain.h"
#include "ofxPatcher.h"
#include "ofxScheduler.h"
#include "ofxAudioOut.h"
#include "ofxAudioIn.h"
#include "ofxFablio.h"

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
	ofxFablioStream *Stream;
	ofxAudioOut DAC;
	ofxAudioIn ADC;
};
