#include "ofMain.h"
#include "testApp.h"

int main( void ) {
	
	ofSetupOpenGL( 1024,768, OF_FULLSCREEN );
	ofRunApp( new testApp() );
}
