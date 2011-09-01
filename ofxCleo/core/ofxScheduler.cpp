#include "ofxScheduler.h"

ofxScheduler::ofxScheduler( void ) {
	
	isPaused = false;
}

ofxScheduler::~ofxScheduler( void ) {
	
	
}

void ofxScheduler::setAudio( bool Mode ) {
	
	isAudioOn = Mode;
}

void ofxScheduler::setPatcher( ofxPatcher *PatcherPtr ) {
	
	Patcher = PatcherPtr;
}

void ofxScheduler::threadedFunction( void ) {
	
	while( threadRunning ) {

		if( !isPaused ) tick();
		if( !isAudioOn ) usleep( 10000 );
	}
}

void ofxScheduler::pause( bool Mode ) {
	
	isPaused = Mode;
}

void ofxScheduler::tick( void ) {

	// propage control and audio processing the the graph
	for( int k=0; k<Patcher->SortedObjectList.size(); k++ ) {
		
		if( Patcher->SortedObjectList[k]->isInControlScheduler ) Patcher->SortedObjectList[k]->_update();
		if( Patcher->SortedObjectList[k]->isInAudioScheduler && isAudioOn ) Patcher->SortedObjectList[k]->_tick();
	}
}
