#pragma once

#include "ofMain.h"

//#define DEBUGMODE

//#define Prototype1
//
//#define Prototype2

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
#ifdef DEBUGMODE
       ofEasyCam camera;
#endif
#ifndef DEBUGMODE
        ofCamera camera;
        ofVec3f cameraStartPosition,cameraEndPosition;
    
        int cameraindex=0;
        bool animationMode;
        ofVec3f animate(int,int);
    
    float tweenvalue,beginAnimationtime;
        ofVec3f tweenedCameraPosition;
        int animationCounter;
    
        void startAnimation();
        bool isstartingAnimationActive;
        ofVec3f animationStartingPosition,animationEndPosition;
        ofVec3f startAnimationCameraPosition();
        int startAnimationCounter;
        int zdistanceFactor;
#endif

    
    
        vector<ofVec3f> SpiralPoints;
        void generateSpiral();
        void generateCircularSpiral();

        void loadImagesFromDirectory();

        vector<ofImage>ImageVector;
        vector<ofVec3f>ImagePositions;

        void AssignRandomPositions();
        void drawImages();

};
