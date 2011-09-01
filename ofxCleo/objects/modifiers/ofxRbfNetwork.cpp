/*
 * ofxRbfNetwork.cpp ~ openFrameworks
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

#include "ofxRbfNetwork.h"

void ofxRbfNetwork::setup( void ) {
	
	//nOfInputs = 0;
	//nOfTargets = 0;
	AllTargets = 0.0;
	SDBoost = 0.2;
	
	isTraining = false;
}

void ofxRbfNetwork::setNetwork( const string &XmlFile ) {
	
	// if the setNetwork() call is used with file as argument,
	// the RBF size and weights are loaded from the file
	
	loadFile( XmlFile );
}

void ofxRbfNetwork::setNetwork( int _nOfInputs, int _nOfTargets ) {
	
	// if the setNetwork() call is used with size arguments,
	// everything is created by hand and initialized to zero
	
	nOfInputs = _nOfInputs; // this RBF network takes input vector
	nOfTargets = _nOfTargets; // and responds target likelihood vector
	
	// create inlets and outlets
	Input.resize( nOfInputs ); Target.resize( nOfTargets );
	for( int k=0; k<nOfInputs; k++ ) setControlInlet( &Input[k], RbfInput+k );
	for( int k=0; k<nOfTargets; k++ ) setControlOutlet( &Target[k], RbfTarget+k );
	
	
	nOfSamples.resize( nOfTargets, 0 ); // keep track of N for each target
	Trained.resize( nOfTargets, false ); // targets not trained by def
	
	Mean.resize( nOfInputs ); // initiate matrix of means
    for( int k=0; k<nOfInputs; k++ ) Mean[k].resize( nOfTargets, 0.0 );
	
	Variance.resize( nOfInputs ); // initiate matrix of variances
    for( int k=0; k<nOfInputs; k++ ) Variance[k].resize( nOfTargets, 0.0 );
	
    SD.resize( nOfInputs ); // initiate matrix of std deviations
    for( int k=0; k<nOfInputs; k++ ) SD[k].resize( nOfTargets, 0.0 );
}

void ofxRbfNetwork::update( void ) {
	
	if( !isTraining ) {
		
		AllTargets = 0.0;
		for( int t=0; t<nOfTargets; t++ ) {
			
			// initialize targets to zero
			Target[t] = 0.0;
		}
		
		// loop on the whole matrix size
		for( int t=0; t<nOfTargets; t++ ) {
			
			for( int m=0; m<nOfInputs; m++ ) {
				
				// first part of gaussian distribution equation
				Target[t] = Target[t] + powf( (Input[m] - Mean[m][t]),
				2.0 ) / ( 2.0 * powf( ( SD[m][t] + SDBoost ), 2.0 ) );
			}
			
			Target[t] = exp( -Target[t] ); // second part of gaussian distribution equation
			AllTargets = AllTargets + Target[t]; // normalized RBF model ~ compute sum
		}
		
		for( int t=0; t<nOfTargets; t++ ) {
			
			// normalized RBF model ~ divide by sum
			Target[t] = Target[t] / AllTargets;
		}
	}
}

void ofxRbfNetwork::addSample( vector<double> &Sample, int TargetId ) {
	
	float PreviousMean = 0.0;
	
	if( Sample.size() != nOfInputs ) {
		
		// if size of input is wrong, we quit
		printf( "ofxRbfNetwork - mismatch input size\n" );
		return;
	}
	
	nOfSamples[TargetId]++; // increment # of samples
	
	for( int m=0; m<nOfInputs; m++ ) {
		
		if( nOfSamples[TargetId] == 1 ) {
			
			// if first sample, just init
			Mean[m][TargetId] = Sample[m];
			Variance[m][TargetId] = 0.0;
			SD[m][TargetId] = 0.0;
			
		} else {
			
			PreviousMean = Mean[m][TargetId]; // compute mean and standard deviation incrementally
			Mean[m][TargetId] = Mean[m][TargetId] + ( Sample[m]-Mean[m][TargetId] )/((float)nOfSamples[TargetId]);
			Variance[m][TargetId] = Variance[m][TargetId] + ( Sample[m]-PreviousMean )*( Sample[m]-Mean[m][TargetId] );
			SD[m][TargetId] = sqrt( Variance[m][TargetId]/((float)(nOfSamples[TargetId]-1)) );
		}
	}
	
	Trained[TargetId] = true;
}

void ofxRbfNetwork::addSample( int TargetId ) {
	
	// this addSample() call uses the instantaneous values
	// from the object input in order to train the network
	
	isTraining = true;
	addSample( Input, TargetId );
	isTraining = false;
}

void ofxRbfNetwork::saveFile( const string &XmlFile ) {
	
	// --- HEADER ---
	Weights.addTag( "header" );
	Weights.setValue( "header:nOfInputs", nOfInputs, 0 );
	Weights.setValue( "header:nOfTargets", nOfTargets, 0 );
	Weights.setValue( "header:SDBoost", SDBoost, 0 );
	
	// --- <WEIGHTS> ---
	Weights.addTag( "weights" );
	Weights.pushTag( "weights", 0 );
	
	// --- <MEANS> ---
	Weights.addTag( "means" );
	Weights.pushTag( "means", 0 );
	
	for( int t=0; t<nOfTargets; t++ ) {
		
		Weights.addTag( "target" );
		Weights.pushTag( "target", t );
		
		for( int m=0; m<nOfInputs; m++ ) {
			
			Weights.addTag( "input" );
			Weights.pushTag( "input", m );
			
			Weights.setValue( "value", Mean[m][t], m );
			
			Weights.popTag();
		}
		
		Weights.popTag();
	}
	
	// --- </MEANS> ---
	Weights.popTag();
	
	// --- <VARIANCES> ---
	Weights.addTag( "variances" );
	Weights.pushTag( "variances", 0 );
	
	for( int t=0; t<nOfTargets; t++ ) {
		
		Weights.addTag( "target" );
		Weights.pushTag( "target", t );
		
		for( int m=0; m<nOfInputs; m++ ) {
			
			Weights.addTag( "input" );
			Weights.pushTag( "input", m );
			
			Weights.setValue( "value", Variance[m][t], m );
			
			Weights.popTag();
		}
		
		Weights.popTag();
	}
	
	// --- </VARIANCES> ---
	Weights.popTag();
	
	// --- <SDS> ---
	Weights.addTag( "sds" );
	Weights.pushTag( "sds", 0 );
	
	for( int t=0; t<nOfTargets; t++ ) {
		
		Weights.addTag( "target" );
		Weights.pushTag( "target", t );
		
		for( int m=0; m<nOfInputs; m++ ) {
			
			Weights.addTag( "input" );
			Weights.pushTag( "input", m );
			
			Weights.setValue( "value", SD[m][t], m );
			
			Weights.popTag();
		}
		
		Weights.popTag();
	}
	
	// --- </SDS> ---
	Weights.popTag();
	
	// --- </WEIGHTS> ---
	Weights.popTag();
	
	// --- SAVING FILE ---
	Weights.saveFile( XmlFile );
}

void ofxRbfNetwork::loadFile( const string &XmlFile ) {
	
	Weights.clear();
	Weights.loadFile( XmlFile );
	
	// --- <HEADER> ---
	Weights.pushTag( "header", 0 );
	
	int _nOfInputs = Weights.getValue( "nOfInputs", 0 );
	int _nOfTargets = Weights.getValue( "nOfTargets", 0 );
	
	if( _nOfInputs != 0 && _nOfTargets != 0 ) {
		
		setNetwork( _nOfInputs, _nOfTargets );
		SDBoost = Weights.getValue( "SDBoost", 0.2 );
		printf( "sdboost: %f\n", SDBoost );
	}
	
	// --- </HEADER> ---
	Weights.popTag();
	
	// --- <WEIGHTS> ---
	Weights.pushTag( "weights", 0 );
	
	// --- <MEANS> ---
	Weights.pushTag( "means", 0 );

	for( int t=0; t<nOfTargets; t++ ) {
		
		Weights.pushTag( "target", t );
		
		for( int m=0; m<nOfInputs; m++ ) {
			
			Weights.pushTag( "input", m );
			Mean[m][t] = Weights.getValue( "value", 0.0 );
			Weights.popTag();
		}
		
		Weights.popTag();
	}
	
	// --- </MEANS> ---
	Weights.popTag();
	
	// --- <VARIANCES> ---
	Weights.pushTag( "variances", 0 );
	
	for( int t=0; t<nOfTargets; t++ ) {
		
		Weights.pushTag( "target", t );
		
		for( int m=0; m<nOfInputs; m++ ) {
			
			Weights.pushTag( "input", m );
			Variance[m][t] = Weights.getValue( "value", 0.0 );
			Weights.popTag();
		}
		
		Weights.popTag();
	}
	
	// --- </VARIANCES> ---
	Weights.popTag();
	
	// --- <SDS> ---
	Weights.pushTag( "sds", 0 );
	
	for( int t=0; t<nOfTargets; t++ ) {
		
		Weights.pushTag( "target", t );
		
		for( int m=0; m<nOfInputs; m++ ) {
			
			Weights.pushTag( "input", m );
			SD[m][t] = Weights.getValue( "value", 0.0 );
			Weights.popTag();
		}
		
		Weights.popTag();
	}
	
	// --- </SDS> ---
	Weights.popTag();
	
	// --- </WEIGHTS> ---
	Weights.popTag();
}

void ofxRbfNetwork::printWeights(void) {
	
    unsigned int m, t;
	
    for ( t = 0; t < nOfTargets; t++ ) {
		
        std::cout << "--- target " << t << " ---" << std::endl;
        
        std::cout << "means: ";
        for ( m = 0; m < nOfInputs; m++ ) std::cout << Mean[m][t] << " ";
        std::cout << std::endl;
		
        std::cout << "stds: ";
        for ( m = 0; m < nOfInputs; m++ ) std::cout << SD[m][t] << " ";
        std::cout << std::endl;
		
        std::cout << std::endl;
    }
}
