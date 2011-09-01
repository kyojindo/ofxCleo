#pragma once

#include <vector>
#include "ofxCleo.h"

using namespace std;

enum { MixerSigIn };
enum { MixerSigOut };

class ofxMixer : public ofxObject {
	
public:
	
	void setup( void );
	void setNumOfInputs( int n );
	void tick( void );
	
	int nOfInputs;
	vector<float *> InBuf;
	float *OutBuf;
};
