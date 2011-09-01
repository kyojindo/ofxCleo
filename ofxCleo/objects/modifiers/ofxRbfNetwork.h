/*
 * ofxRbfNetwork.h ~ openFrameworks
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
