#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include <algorithm>  
#include "ofxBlur.h"
#include "ofMain.h"

#include "ofxOsc.h"
# define USEWII


//#define DEBUGMODE

//#define Prototype1
//
//#define Prototype2

//#define BLUR


#define PORT 9000

#define FINDRANGE


#define EfficientReorder

struct ImageData
{
    float imageScore;
    int albumnumber;
    int imageNumber;
    
#ifdef EfficientReorder
    ofImage theloadedimage;
#else
   bool isTagged;
#endif
 
   bool operator()(const ImageData&l,const ImageData &m) {
        return (l.imageScore<m.imageScore);
       }
     
};

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
    
//        multimap<int,int>taggedimageScores;
//        multimap<int,int>untaggedimageScores;
//    
//        multimap<int,int>taggedalbumScores;
//        multimap<int,int>untaggedalbumScores;
    
        vector<ImageData> imageDetails,taggedImages,untaggedImages;
    
        vector<ImageData> untaggedImageObjects,taggedImageObjects,combinedImageObjects;
    
    
    
        bool sortOnImageScore(ImageData l,ImageData r);
    
        vector<ofVec3f> imageData;
        void newReorder();
        void complexReorder();
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
    
        void loadImagesandXMLData();
        int lengthofImages;

    
        ofTrueTypeFont NostalgiaFont;
    
        float timeSincePreviousAnimation;
    
        ofVec3f overshotCameraStartingPosition;
        ofVec3f adjustoverShotCameraPosition();
        bool startoverShotCameraAnimation;
    
    ///// Blur the Photos ....
    
#ifdef BLUR
    ofxBlur blur;
#endif
    
    float position1_z,position2_z;

#ifdef REPETITION_TESTING
        vector<int>imageIndices;
#endif

        ofQTKitPlayer startingMovie;
        bool startingMovieFinished;
    
    
        ofSoundPlayer BluementhalMp3;
    
 
#ifdef USEWII
    
// Wii-Mote Variables ....
    
    float roll, yaw, pitch, accel, wiiX, wiiY;
    
    float accel_x,accel_y,accel_z;
    float angular_velocity;
    ofxOscReceiver receiver;
    
    string Message;
      int w, h;
    
 
    
#ifdef FINDRANGE
    
    float min,max;
    float minAccel,maxAccel;
    
#endif
    
    ofTrueTypeFont font;
    float u,s,t;
    float Position,Amplitude;
    string State;
#endif
    
    float timeGap;
    bool isturnCompleted;
    float maxAccelinTurn;
    float prevAngVel=0;
    float currentVolume;
    bool fadeAudio;
    
    bool startInstallation;
    bool checkforaccel;
    
    ofTrueTypeFont previewText;
    string tempText;
    
    bool ending;
    
    /// End of Wii-Mote Variables ..

};
