#pragma once

#include <vector>
#include "ofxObject.h"
#include "ofxFablio.h"

using namespace std;

enum { AdcOut }; // inlets for mono or "+k" notation

class ofxAudioIn : public ofxObject {
	
public:
	
	ofxAudioIn( void );
	virtual ~ofxAudioIn( void );
	
	void setStream( ofxFablioStream *stream );
	
	void setup( void );
	void tick( void );
	
	ofxFablioStream *Stream;
	float *InterlacedBuffer;
	vector<float *> OutBuf;
};
