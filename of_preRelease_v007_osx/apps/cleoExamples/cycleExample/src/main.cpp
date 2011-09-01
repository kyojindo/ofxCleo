#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "testApp.h"

int main( void ) {

    ofAppGlutWindow window;
	ofSetupOpenGL( &window, 1024, 768, OF_WINDOW );
	ofRunApp( new testApp() );
}
