#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include <algorithm>  
#include "ofxBlur.h"
#include "ofMain.h"

#include "ofxOsc.h"

# define USEWII

// The ofxWiiOSC addon will listen to messages on this port

#define PORT 9000

// If the below parameter is defined,the timegap between two images will be adjusted .If the acceleration of the wii-mote is large,the user is swinging faster and the timegap is made small.If the user slows down,it means he would like to look at the image for more time,so the timegap is made large.

#define ADJUSTTIMEGAP

#define EfficientReorder


//#define DEBUGMODE

//#define Prototype1
//
//#define Prototype2

//#define BLUR


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
    
        ofCamera camera; // This is the camera variable that will move around the entire scene 
        ofVec3f cameraStartPosition,cameraEndPosition;
    
        int cameraindex=0;
        bool animationMode;
    
        ofVec3f animate(int,int); // Given 2 indices (that are positions in a conical helix,this Function will animate the camera from one position to another using ofLerp.
    
        float tweenvalue,beginAnimationtime;
    
        ofVec3f tweenedCameraPosition; // Stores the CameraPosition while tweening 
    
        int animationCounter;
    
        void startAnimation();
        bool isstartingAnimationActive; // StartingAnimation is the first animation that zooms out of the spiral ..
    
        ofVec3f animationStartingPosition,animationEndPosition;
        ofVec3f startAnimationCameraPosition(); // This function will return a Vector that will animate the camera to 
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
    
        int currentwiggleindex; 
    
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
    
    /* Wii-Mote Variables */
    
    
    /* Roll,Yaw and Pitch as sent by OSCulator */
    float roll, yaw, pitch, accel, wiiX, wiiY;
    
    /* Acceleration in x,y and z*/
    float accel_x,accel_y,accel_z;
    
    /* Angular Velocity as received from OSCulator ,this variable is helpful to predict if the swing is going forwards or backward .A +ve value means the swing is going front and a -ve value indicates the person is swinging backwards */
    float angular_velocity;
    
    /* OSCReceiver class that interfaces with the OSCulator and receives values from the wii-mote */
    ofxOscReceiver receiver;
    
    string Message;
    int w, h;
    
    /* End of Wii-Mote Variables */
    
#ifdef ADJUSTTIMEGAP
    
    float min,max;
    float minAccel,maxAccel;
    
#endif
    
    ofTrueTypeFont font; 
    float u,s,t;
    float Position,Amplitude;
    string State;
#endif
    
    /* Time that an image will be shown on the screen,this is controlled by the wii-motes acceleration */
    float timeGap;
    
    
    /* This variable is used to check if a half-swing is completed ,that is from back to front or from front to back */
    bool isturnCompleted;
    
    // UNUSED
    float prevAngVel=0;
    
    
    /* If set,this variable will cause the currentVolume variable to fade */
    bool fadeAudio;
    
    /* The current volume variable is generally set to 1 ,but if fadeAudio is enabled,it will reduce the audio by -0.001 every frame,this is used in the end along with the Corresponding video to reduce the volume of the music */
    float currentVolume;
    
  
    
    /* This variable is used to start the installation if the user is swinging on the wii-mote or if the enter key is pressed */
    bool startInstallation;
    
    bool checkforaccel;
    
    /* 
     * The below variables are used in the starting if the person is not swinging 
     */
    
    /* previewText is used for loading the font Asyouwish.ttf */
    ofTrueTypeFont previewText;
    
    /* tempText will just store the message "Start Swinging !" and display it in the start */
    string tempText;
    
    /* If true,the ending variable will trigger the backward playing of the starting video and provides a nice smooth ending transition of the experience  */
    bool ending;
    

};
