#pragma once

#include <vector>
#include "ofxObject.h"
#include "ofxFablio.h"

using namespace std;

enum { DacIn }; // inlets for mono or "+k" notation
enum { DacLeft, DacRight }; // inlets for stereo

class ofxAudioOut : public ofxObject {
	
  public:
	
	ofxAudioOut( void );
	virtual ~ofxAudioOut( void );
	
	void setStream( ofxFablioStream *stream );
	
	void setup( void );
	void tick( void );
	
	ofxFablioStream *Stream;
	float *InterlacedBuffer;
	vector<float *> InBuf;
};
