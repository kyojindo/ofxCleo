#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxCleo.h"
#include "ofxXmlSettings.h"

enum { RbfInput };
enum { RbfTarget };

class ofxRbfNetwork : public ofxObject {
	
  public:
	
	void setup( void );
	void update( void );
	
	void setNetwork( const string &XmlFile );
	void setNetwork( int _nOfInputs, int _nOfTargets );
	
	void addSample( vector<double> &Sample, int TargetId );
	void addSample( int TargetId );
	
	void saveFile( const string &XmlFile );
	void loadFile( const string &XmlFile );
	
	void printWeights( void );
	
	int nOfInputs;
	int nOfTargets;
	double SDBoost;
	
	vector<double> Input;
	vector<double> Target;
	vector<bool> Trained;
	
	vector<int> nOfSamples;
	float AllTargets;
	
	vector< vector<double> > Mean;
	vector< vector<double> > Variance;
	vector< vector<double> > SD;
	
	ofxXmlSettings Weights;
	bool isTraining;
};
