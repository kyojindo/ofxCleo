#include "ofxObject.h"

ofxObject::ofxObject( void ) {
	
	Type = ofxObjectParent; // parent = default
	isAttached = false; // not attached by default
	nOfTargets = 0; TokenCount = 0; // default graph values
	
	isInControlScheduler = false; // by default the object is not
	isInAudioScheduler = false; // scheduler in control or audio
	isThreaded = false; // neither a threaded object
}

ofxObject::~ofxObject( void ) {
	
	if( isThreaded ) {
		
		// stop and delete thread
		ObjectThread->waitForThread( true );
		delete ObjectThread;
	}

	for( int k=0; k<aOutlet.size(); k++ )
		free( aOutlet[k] ); // free audio outlets
}

void ofxObject::resetToken( void ) {
	
	TokenCount = 0;
}

void ofxObject::incrementToken( void ) {
	
	TokenCount++;
}

void ofxObject::emitToken( void ) {
	
	// for all following objects
	for( int k=0; k<Target.size(); k++ ) {
		
		if( Target[k] != NULL ) {
			
			Target[k]->incrementToken(); // increment
			Target[k]->emitToken(); // and transfer
		}
	}
}

void ofxObject::resizeInletBuffers( int Length ) {
	
	InletType.resize( Length ); // resize inlet type buffer
	kInletPatched.resize( Length ); // resize control inlet-patched buffer
	kInletLookup.resize( Length ); // resize control inlet lookup buffer
	kInletThreaded.resize( Length ); // resize control inlet-threaded buffer
	kInletMultiplier.resize( Length ); // resize control inlet multiplier
	kInletOffset.resize( Length ); // resize control inlet offset
	kInlet.resize( Length ); // resize control inlet pointer buffer
	aInlet.resize( Length ); // resize audio inlet pointer buffer
}

void ofxObject::resizeOutletBuffers( int Length ) {
	
	OutletType.resize( Length ); // resize control outlet type buffer
	kOutletBackup.resize( Length ); // resize control outlet backup buffer
	kOutletThreaded.resize( Length ); // resize control outlet-threaded buffer
	kOutlet.resize( Length ); // resize control outlet pointer buffer
	aOutlet.resize( Length ); // resize audio outlet pointer buffer
}

void ofxObject::setControlInlet( double *Variable, ofxObjectField Field ) {
	
	// when a control inlet is suggested, we extend
	// all the inlet buffers (control and audio) and
	// adapt the mask to specify it's a control one

	// resize all inlet buffers if new field > max size
	if( InletType.size() < Field+1 ) resizeInletBuffers( Field+1 );
	
	// -- configure buffers for a new control inlet --
	InletType[ Field ] = ofxIoletControl; // set to control inlet type
	kInletPatched[ Field ] = false; // set inlet not patched by default
	kInletLookup[ Field ] = NULL; // set inlet lookup pointing to null
	kInletThreaded[ Field ] = false; // set inlet not threaded by default
	kInletMultiplier[ Field ] = 1.0; // set inlet multiplier to one
	kInletOffset[ Field ] = 0.0; // set inlet offset to zero
	kInlet[ Field ] = Variable; // set inlet to point to user variable
	aInlet[ Field ] = NULL; // here audio inlet is abandonned
	
	isInControlScheduler = true; // added to control scheduler
}

void ofxObject::setControlOutlet( double *Variable, ofxObjectField Field ) {

	// when a control outlet is suggested, we extend
	// all the outlet buffers (control and audio) and
	// adapt the mask to specify it's a control one
	
	// resise all outlet buffers if new field > max size
	if( OutletType.size() < Field+1 ) resizeOutletBuffers( Field+1 );
	
	// -- configure buffers for a new control outlet --
	OutletType[ Field ] = ofxIoletControl; // set to control outlet type
	kOutletBackup[ Field ] = 0.0; // set outlet backup to start with zero
	kOutletThreaded[ Field ] = false; // set outlet not threaded by default
	kOutlet[ Field ] = Variable; // set outlet to point to user variable
	aOutlet[ Field ] = NULL; // here audio outlet is abandonned
	
	isInControlScheduler = true; // added to control scheduler
}

void ofxObject::setAudioInlet( float **Buffer, ofxObjectField Field ) {
	
	// when an audio inlet is suggested, we extend
	// all the inlet buffers (control and audio) and
	// adapt the mask to specify it's an audio one
	
	// resize all inlet buffers if new field > max size
	if( InletType.size() < Field+1 ) resizeInletBuffers( Field+1 );
	
	// -- configure buffers for a new audio inlet --
	InletType[ Field ] = ofxIoletAudio; // set to audio inlet type
	kInletPatched[ Field ] = false; // set inlet not patched by default
	kInletLookup[ Field ] = NULL; // set inlet lookup pointing to null
	kInletThreaded[ Field ] = false; // inlet not threaded by default
	kInlet[ Field ] = NULL; // here control inlet is abandonned
	
	aInlet[ Field ] = Buffer; // save pointer to pointer in the buffer
	*( aInlet[ Field ] ) = aNoInlet; // and redirect it first to zero
	
	isInAudioScheduler = true; // added to audio scheduler
}

void ofxObject::setAudioOutlet( float **Buffer, ofxObjectField Field ) {
	
	// resise all outlet buffers if new field > max size
	if( OutletType.size() < Field+1 ) resizeOutletBuffers( Field+1 );
	
	// -- configure buffers for a new audio outlet --
	OutletType[ Field ] = ofxIoletAudio; // set to audio inlet type
	kOutletBackup[ Field ] = 0.0; // set outlet backup to start with zero
	kOutletThreaded[ Field ] = false; // set outlet not threaded by default
	kOutlet[ Field ] = NULL; // here control outlet is abandonned
	
	// create a new outlet buffer for audio samples
	aOutlet[ Field ] = (float *) malloc( tickBufferSize*sizeof(float) );
	*Buffer = aOutlet[ Field ]; // redirect user-specified pointer
	
	isInAudioScheduler = true; // added to audio scheduler
}

void ofxObject::setAsThreaded( void ) {
	
	isThreaded = true;
}

void ofxObject::setup( void ) {
	
	// this space has to be
	// overwriten by inheritance
}

void ofxObject::_setup( void ) {
	
	setup();
	
	if( isThreaded ) {
		
		ObjectThread = new ofxObjectThread( this ); // create the thread
		Type = ofxObjectThreaded; // type the object as threaded
		
		isInControlScheduler = false; // threaded object is removed from
		isInAudioScheduler = false; // both control & audio schedulers
		
		ObjectThread->startThread( false, false ); // start the thread
	}
}

void ofxObject::_update( void ) {
	
	// -- synchronizing inlets --
	for( int k=0; k<kInlet.size(); k++ ) {
		
		if( kInletPatched[k] && InletType[k] == ofxIoletControl ) {
			
			if( kInletThreaded[k] == true ) WriteMemoryBarrier();
			*( kInlet[k] ) = ( *( kInletLookup[k] ) * kInletMultiplier[k] ) + kInletOffset[k];
		}
	}
	
	update(); // --- UPDATE ---
	
	// -- synchronizing outlets --
	for( int k=0; k<kOutlet.size(); k++ ) {
		
		if( OutletType[k] == ofxIoletControl ) {
			
			if( kOutletThreaded[k] == true ) WriteMemoryBarrier();
			kOutletBackup[k] = *( kOutlet[k] );
		}
	}
}

void ofxObject::update( void ) {
	
	// this space has to be
	// overwriten by inheritance
}

void ofxObject::_tick( void ) {
	
	tick();
}

void ofxObject::tick( void ) {
	
	// this space has to be
	// overwriten by inheritance
}

ofxObjectThread::ofxObjectThread( ofxObject *ObjectPtr ) {
	
	Object = ObjectPtr;
}

ofxObjectThread::~ofxObjectThread( void ) {
	
	
}

void ofxObjectThread::threadedFunction( void ) {
	
	while( threadRunning ) {
		
		// the threaded object updates its own control state
		// independantly from the scheduler ( asynchronous )
		Object->_update();
	}
}
