/*
 * ofxPatcher.cpp ~ openFrameworks
 * Nicolas d'Alessandro <nicolas@dalessandro.be>
 * and Johnty Wang <info@johnty.ca>
 *
 * ofxCleo is an audio patching utiliy for openFrameworks.
 * It allows to connect inlets and outlets from various kinds
 * of objects ( controllers, modifiers, generators, filters )
 * into a graph and solve it in realtime. ofxCleo also allows
 * to simply create new objects to be patched by inheritance.
 *
 *
 * MIT License ~ Copyright (c) 2011 University of British Columbia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ofxPatcher.h"

ofxPatcher::ofxPatcher( void ) {
	
	
}

ofxPatcher::~ofxPatcher( void ) {
	
	
}

void ofxPatcher::attachToObjectList( ofxObject &Object ) {
	
	// synchronize parameters
	Object.sampleRate = sampleRate;
	Object.tickBufferSize = tickBufferSize;
	Object.ioBufferSize = ioBufferSize;
	Object.aNoInlet = aNoInlet;
	
	Object._setup(); // trigger object setup
	
	// attach object to patcher object list
	ObjectList.resize( ObjectList.size()+1 );
	SortedObjectList.resize( ObjectList.size() );
	ObjectList[ ObjectList.size()-1 ] = &Object;
	
	Object.isAttached = true; // is attached
}

void ofxPatcher::solveObjectGraph( ofxObject &Source, ofxObject &Target ) {
	
	Source.nOfTargets++; // resize the vector
	Source.Target.resize( Source.nOfTargets );
	
	// connecting target to new target pointer
	Source.Target[ Source.nOfTargets-1 ] = &Target;
	
	// -- reset all tokens --
	for( int k=0; k<ObjectList.size(); k++ ) {
		
		ObjectList[k]->resetToken();
	}
	
	// -- propagate tokens --
	for( int k=0; k<ObjectList.size(); k++ ) {
		
		ObjectList[k]->emitToken();
	}
	
	// -- find highest token --
	int highestToken = 0;
	for( int k=0; k<ObjectList.size(); k++ ) {
		
		if( ObjectList[k]->TokenCount > highestToken ) {
			
			highestToken = ObjectList[k]->TokenCount;
		}
	}
	
	// -- reorganizing by token value --
	int pos = 0;
	for ( int m=0; m<highestToken+1; m++ ) {
		
		for( int k=0; k<ObjectList.size(); k++ ) {
			
			if( ObjectList[k]->TokenCount == m ) {
				
				SortedObjectList[pos] = ObjectList[k];
				pos++;
			}
		}
	}
}

void ofxPatcher::patch( ofxObject &Source, ofxObjectField SourceField, ofxObject &Target, ofxObjectField TargetField,
 double SourceLowBound, double SourceHighBound, double TargetLowBound, double TargetHighBound ) {
	
	// if first patching for source or target: setup and attach
	if( Source.isAttached == false ) attachToObjectList( Source );
	if( Target.isAttached == false ) attachToObjectList( Target );
	
	// sort list by solving graph
	solveObjectGraph( Source, Target );
	
	if( Source.OutletType[ SourceField ] == ofxIoletControl &&
	    Target.InletType[ TargetField ] == ofxIoletControl ) {
	
		// -- do control rate patching --
		Target.kInletLookup[ TargetField ] = &( Source.kOutletBackup[ SourceField ] );
		
		// -- apply the mapping values --
		Target.kInletOffset[ TargetField ] = ((TargetLowBound-TargetHighBound)/(SourceHighBound-SourceLowBound))*SourceLowBound + TargetLowBound;
		Target.kInletMultiplier[ TargetField ] = (TargetHighBound-TargetLowBound)/(SourceHighBound-SourceLowBound);
		
		// -- protect the parch link if either source or target is threaded --
		if( Source.Type == ofxObjectThreaded || Target.Type == ofxObjectThreaded ) {
			
			Source.kOutletThreaded[ SourceField ] = true;
			Target.kInletThreaded[ TargetField ] = true;
		}
		
		Target.kInletPatched[ TargetField ] = true;
		
	} else if( Source.OutletType[ SourceField ] == ofxIoletAudio &&
			   Target.InletType[ TargetField ] == ofxIoletAudio ) {
		
		// -- do audio rate patching --
		*( Target.aInlet[ TargetField ] ) = Source.aOutlet[ SourceField ];
		
	} else {
		
		printf("ofxPatcher - type mismatch between in and out\n");
	}
}

void ofxPatcher::attach( ofxObject &Object ) {
	
	if( Object.isAttached == false ) attachToObjectList( Object );
}
