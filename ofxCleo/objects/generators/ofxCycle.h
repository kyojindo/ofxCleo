#include "ofMain.h"
#include "ofxCleo.h"

enum { CyclePitch, CycleVolume }; // inlets
enum { CycleSigOut }; // outlets

class ofxCycle : public ofxObject {
	
public:
	
	void setup( void );
	void update( void );
	void tick( void );
	
	double phase;
	double Pitch, PitchInHz, Volume;
	float *OutBuf;
};
