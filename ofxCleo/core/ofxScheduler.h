#pragma once

#include <ctime>
#include "ofxObject.h"
#include "ofxPatcher.h"

#if( OF_VERSION < 7 )
#include "ofxThread.h"
#else
#include "ofThread.h"
#endif

#if( OF_VERSION < 7 )
typedef ofxThread ofThread;
#endif

class ofxScheduler : public ofThread {
	
  public:
	
	ofxScheduler( void );
	virtual ~ofxScheduler( void );
	
	void setAudio( bool Mode );
	void setPatcher( ofxPatcher *PatcherPtr );
	
	void threadedFunction( void );
	
	void pause( bool Mode );
	
	void tick( void );
	
	ofxPatcher *Patcher;
	bool isAudioOn;
	bool isPaused;
};
