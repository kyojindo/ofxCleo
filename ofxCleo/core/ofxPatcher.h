#pragma once

#include <vector>
#include "ofxObject.h"

using namespace std;

class ofxPatcher {
	
  public:
	
	ofxPatcher( void );
	virtual ~ofxPatcher( void );
	
	void attachToObjectList( ofxObject &Object );
	void solveObjectGraph( ofxObject &Source, ofxObject &Target );
	
	void patch( ofxObject &Source, ofxObjectField SourceField, ofxObject &Target, ofxObjectField TargetField,
	 double SourceLowBound, double SourceHighBound, double TargetLowBound, double TargetHighBound );
	void attach( ofxObject &Object );
	
	vector<ofxObject *> SortedObjectList;
	vector<ofxObject *> ObjectList;
	
	int sampleRate;
	int ioBufferSize;
	int tickBufferSize;
	float *aNoInlet;
};
