#pragma once

#include "ofMain.h"
#include "ofxCleo.h"

enum { RandSpeed, RandMin, RandMax }; // list inlets
enum { RandOut }; // list outlets

class ofxRand : public ofxObject {
	
public:
	
	void setup( void );
	void update( void );
	
	double Speed;
	double Min;
	double Max;
	double Res;
};
