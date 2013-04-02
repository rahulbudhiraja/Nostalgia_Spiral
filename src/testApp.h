#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include <algorithm>  

//#define DEBUGMODE

//#define Prototype1
//
//#define Prototype2

# define REPETITION_TESTING

class testApp : public ofBaseApp{
	public:
    testApp(long long int id);
    
    
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
    
        void sortImages();
        multimap <int, int > imageScores;
    
        ofxXmlSettings pictures_XML;
        long long int userid;
    
    
    /* The Wiggling Stuff */
        vector <ofVec3f> wigglePositions;
    
        void pushWigglePositions(); /// Pushing values ...
    
        ofVec3f wiggle(); // Actual wiggle code ...similar to the ofLerp for the animations ...
    
        ofVec3f currentwigglePosition; // Current Position of the camera ...
    
        int currentwiggleindex; // the index of the position in the Wiggle Positions vector ...
    
        float wiggleAnimationCounter;
        void reorder();
    
        /* End of Wiggling stuff. */
    
    
        multimap <int, int > albumScores;
        vector<ofVec3f> imageData;
        void newReorder();
        void assignStarPositions();
        void drawStars();
    
        vector<ofVec3f>StarPositions;
    
#endif
    
        vector<ofVec3f> SpiralPoints;
        void generateSpiral();
        void generateCircularSpiral();

        void loadImagesFromDirectory();

        vector<ofImage>ImageVector;
        vector<ofVec3f>ImagePositions;

        void AssignRandomPositions();
        void drawImages();
        int blahblah;
    
        ofTrueTypeFont NostalgiaFont;
    

#ifdef REPETITION_TESTING
        vector<int>imageIndices;
#endif

};
