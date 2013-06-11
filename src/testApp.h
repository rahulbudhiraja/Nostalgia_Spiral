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
    
        /* This is the camera variable that will move around the entire scene */
        ofCamera camera;
    
        /* These variables will be useful if DEBUGMODE is defined */
        ofVec3f cameraStartPosition,cameraEndPosition;
    
        int cameraindex=0;
        bool animationMode;
    
        /* Given 2 indices (that are positions in a conical helix,this Function will animate the camera from one position to another using ofLerp */
        ofVec3f animate(int,int);
    
        float tweenvalue,beginAnimationtime;
    
        /* Stores the temporary CameraPosition while tweening(interpolating values while moving from 1 image/point to another) */
        ofVec3f tweenedCameraPosition; 
    
        int animationCounter;
    
        /* StartingAnimation is the first animation that zooms out of the spiral */
        void startAnimation();
    
        /* If set,this variable will start the Animation of the spiral zooming out */
        bool isstartingAnimationActive;
    
        /* If the starting animation is active,this function will return a vector that has the startingCamera's tweened position */
        ofVec3f startAnimationCameraPosition();
    
        /* During the Starting Animation,the StartAnimationCounter will help in changing the tweenvalue which in turn set the CameraPosition */
        int startAnimationCounter;
    
        /* This was an old function which was used to sort the images */
        void sortImages();
    
        /* A multimap is used because it is easier to store 2 elements(Image number and index) and then sort the whole array based on the score */
        multimap <int, int > imageScores;
    
        /* To load the XML File that is downloaded by the NostalgiaRoom Website */
        ofxXmlSettings pictures_XML;
    
        /* The user's id that is passed at run-time.Only a long long int (max value 9223372036854775807) can support the range of the userids provided by Facebbok */
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
    
        /* Used with the old re-ordering */
        multimap <int, int > albumScores;
    
        /* The below data structures were used with the early re-ordering algorithms */
        vector<ImageData> imageDetails,taggedImages,untaggedImages;
    
        /* Unused Data Structures that were used earlier ,when the XML file was different to rank the images */
        vector<ImageData> untaggedImageObjects,taggedImageObjects;
    
        /* The actual data structure that is used for storing the ImageData objects */
        vector<ImageData> combinedImageObjects;
    
        /* Unused function that returns the lowest score of 2 imageData objects */
        bool sortOnImageScore(ImageData l,ImageData r);
    
        /* Unused Data Structure */
        vector<ofVec3f> imageData;
    
        /* The following are unused data structures */
        void newReorder();
        void complexReorder();
    
        /* Had tried drawing small dot stars in the background.Unfortunately,it was too memory intensive on my machine,including these functions if anyone is interested in trying them out .Just call assiginStarpositions() in the setup function and drawStars() in the draw function */
    
        void assignStarPositions();
        void drawStars();
    
        /* Stores the positions of the stars */
        vector<ofVec3f>StarPositions;
    
#endif
    
        /* These are Spiralpoints generated using the Conical Helix function */
        vector<ofVec3f> SpiralPoints;
    
        /* Function that will generate the points on the Spiral */
        void generateCircularSpiral();

        /* This funtion will load the images from the directory and push them into an ImageVector.UNUSED in the current version of the program */
        void loadImagesFromDirectory();

        /* Will store the loaded images from the directory in a Vector */
        vector<ofImage>ImageVector;
    
        /* This function will draw the images .Used in draw */
        void drawImages();
    
        /* Load the Images and XML Data.This function will load the images from the directory and then parse the xml to get the score and album number.Once that is done ,we combine the untagged and tagged images to get a combined Data Structure  */
        void loadImagesandXMLData();
    
        /* Number of images loaded in the ImageVector data structure */
        int numberofImages;
    
        /* Time since Previous Transition/Animation */
        float timesinceLastTransition;
    
        /* The whole starting Animation consists of a few Components.A video played at the start,The zooming out effect from the end of the spiral and an overshot animation that will take the Camera to the highest ranked image that will be shown first during the experience. The below data structures are for for the OvershotCameraAnimation*/
    
        /* This vector will store the overshotCameraPosition and changes the cameraPosition during the start while tweening */
        ofVec3f overshotCameraStartingPosition;
    
        /* Vector that will use the above vector and ofLerp to compute the exact cameraposition */
        ofVec3f adjustoverShotCameraPosition();
    
        /* When set,this will start the overshot camera animation */
        bool startoverShotCameraAnimation;
   
#ifdef BLUR
    /* Blur the Photos */
    ofxBlur blur;
#endif
    
    /* Intermediate values that are used while tweening/animation of the camera positions */
    float position1_z,position2_z;

    /* Apparently,the ofQTKitPlayer plays HD Video better than ofVideoPlayer */
    ofQTKitPlayer startingMovie;
    
    /* Variable determines if the startingMovie has finished playing or not (initially set to false ) */
    bool startingMovieFinished;
    
    /* This class plays the Bluementhal song by Ulrich Schnauss in the background while the user watches his/her pictures.It is */
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
    
    /* The message received from OSCulator containing details of the wiimote values like angular velocity and acceleration */
    string Message;
    
    int windowWidth,windowHeight;
    
    /* End of Wii-Mote Variables */
    
#ifdef ADJUSTTIMEGAP
  
    /* These are the wii-mote variables that are used to adjust the timegap between two images */
    float minAngularVelocity,maxAngularVelocity;
    float minAccel,maxAccel;
    
#endif
    
    ofTrueTypeFont font; 
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
