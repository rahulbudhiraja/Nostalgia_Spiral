/*! \mainpage NostalgiaRoom Documentation
 *  \section Introduction
 *   The Code takes input from the Wiimote over OSC using OSCUlator and Uses this data to adjust the time an image is shown on the screen.
 *   To setup OSCulator refer: https://sites.google.com/site/ofauckland/examples/18-wiimote-via-ofxosc . The Wii-mote was placed below the swing.
 *   If you do not want to use the wiimote and instead want to use the up and down arrow keys,Just comment the \link USEWII \endlink option in testapp.h.
 *
 */


#include "testApp.h"
#include "ofAppGlutWindow.h"

#include <stdlib.h> 
#include<string.h>

int main(int argc,char *argv[])
{

    if(argc<2)
    {
        cout<<"Too Few Arguements\n";
        ofExit();
    }
    
    char *test;
    
   long long int userid=strtoll(argv[1], &test, 10);
        
   	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	
    ofRunApp(new testApp(userid)); // start the app
}

