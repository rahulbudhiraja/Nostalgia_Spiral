#include "testApp.h"
#include "ofAppGlutWindow.h"

#include <stdlib.h> 
#include<string.h>
//--------------------------------------------------------------
int main(int argc,char *argv[]){

    if(argc<2)
    {
        cout<<"Too Few Arguements\n";
        ofExit();
    }
    
    char *test;
    
   long long int userid=strtoll(argv[1], &test, 10);
    cout<<endl<<userid;

    //    if(argc)
//        userid=100002627332238;
//    else userid=100002627332238;
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	
    ofRunApp(new testApp(userid)); // start the app
}

