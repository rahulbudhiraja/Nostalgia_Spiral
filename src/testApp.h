/** @file */

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include <algorithm>
#include "ofMain.h"
#include "ofxOsc.h"

/** \brief Comment USEWII to disable input from the wiimote */
# define USEWII

/** \brief Comment ADJUSTTIMEGAP to ignore input from the wiimote and keep a standard time for which the image will be shown on the screen */
# define ADJUSTTIMEGAP

/** \brief The ofxWiiOSC addon will listen to messages on this port */

#define PORT 9000

// If the below parameter is defined,the timegap between two images will be adjusted .If the acceleration of the wii-mote is large,the user is swinging faster and the timegap is made small.If the user slows down,it means he would like to look at the image for more time,so the timegap is made large.


#define EfficientReorder

//#define DEBUGMODE

#define PathToImageFolder "/Users/rahulbudhiraja/Work/of_v0073_osx_release/apps/myApps/Nostalgia_Spiral/bin/data/Images/"


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
       
    /**
     * @addtogroup OpenframeworksDefaults
     * @{
     */
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
    /** @} */
    
    
    /**
     The Constructor takes the facebook userid and looks for that particular uid in the Images folder
     @param a long long it
     */
        testApp(long long int uid);
    
    
#ifdef DEBUGMODE
       ofEasyCam camera;
#endif
#ifndef DEBUGMODE
    
        /**
         * @addtogroup Camera
         * \brief This is the camera variable that will move around the entire scene 
         *
         * @{
         */
        ofCamera camera;
    
        /** \brief These variables will be useful if DEBUGMODE is defined */
        ofVec3f cameraStartPosition,cameraEndPosition;
    
        /** \brief The Cameraindex value,this make the camera go from one image/position to another */
        int cameraindex=0;
    
        /** @} */
    
        /**
         * @addtogroup CameraAnimation
         * \brief This are the variables that are used for animating the camera
         * @{ */

        /** \brief checks whether the camera is currently transitioning to another picture or if it is stationary a.k.a animationMode */
        bool animationMode;
    
        /** \brief stores intermediate tweenvalues for all animations */
        float tweenvalue;
    
        /** \brief Stores the temporary camera position while tweening(interpolating camera position while moving from 1 point in the spiral to another) */
        ofVec3f tweenedCameraPosition;
    
        /** \brief Animation Counter that is operative while the animation is occuring */
        int animationCounter;
        
        /** \brief If set,this variable will start the Animation of the spiral zooming out */
        bool isstartingAnimationActive;
               
        /** \brief During the starting animation,the StartAnimationCounter will change the tweenvalue which in turn sets the CameraPosition */
        int startAnimationCounter;
    
        /** @} */
    
    
        /** \brief To load the XML File that is downloaded by the NostalgiaRoom Website  */
        ofxXmlSettings pictures_XML;
    
        /** \brief The user's id that is passed at run-time.Only a long long int (max value 9223372036854775807) can support the range of the userids provided by Facebbok  */
        long long int userid;
        
        /**
         * @addtogroup Wiggle 
         * 
         *@{ */
    
        /** \brief Different wiggle Positions/Directions .Look at the wiggle() function for more details */
        vector <ofVec3f> wigglePositions;
    
        /** \brief Current Wiggle POsition */
        ofVec3f currentwigglePosition;
    
        /** \brief The current active index of the WigglePositions vector.The index determines the direction in which the image will move */
        int currentwiggleindex;
    
        /** \brief A seperate animation counter for wiggling */
        float wiggleAnimationCounter;
    
        /** @} */
    
        /* End of Wiggling stuff. */
    
        /** \brief The actual data structure that is used for storing the ImageData objects */
        vector<ImageData> combinedImageObjects;
    
        /** \brief Stores the positions of the stars */
        vector<ofVec3f>StarPositions;
    
#endif
    
        /** \brief These are Spiralpoints generated using the Conical Helix function */
        vector<ofVec3f> SpiralPoints;
    

        /** \brief Will store the loaded images from the directory in a Vector */
        vector<ofImage>ImageVector;
    
        /** \brief Number of images loaded in the ImageVector data structure */
        int numberofImages;
    
        /** \brief Time since Previous Transition/Animation */
        float timesinceLastTransition;
    
        /** \brief The whole starting Animation consists of a few Components.A video played at the start,The zooming out effect from the end of the spiral and an overshot animation that will take the Camera to the highest ranked image that will be shown first during the experience. The below data structures are for for the OvershotCameraAnimation*/
    
        /** \brief This vector will store the overshotCameraPosition and changes the cameraPosition during the start while tweening */
        ofVec3f overshotCameraStartingPosition;
    
        /** \brief When set,this will start the overshot camera animation */
        bool startoverShotCameraAnimation;
    
        /** \brief Intermediate values that are used while tweening/animation of the camera positions */
        float position1_z,position2_z;

        /** \brief Apparently,the ofQTKitPlayer plays HD Video better than ofVideoPlayer */
        ofQTKitPlayer startingMovie;
        
        /** \brief Variable determines if the startingMovie has finished playing or not (initially set to false ) */
        bool startingMovieFinished;
        
        /** \brief This class plays the Bluementhal song by Ulrich Schnauss in the background while the user watches his/her pictures.It is */
        ofSoundPlayer BluementhalMp3;
        
     
    #ifdef USEWII
        
        /** @addtogroup WiiMote
         * 
         * These variables are used for receiving values from the wiimote
         * @{ 
         */
        
        /** \brief Roll,Yaw and Pitch as sent by OSCulator */
        float roll, yaw, pitch, accel, wiiX, wiiY;
        
        /** \brief Acceleration in x,y and z*/
        float accel_x,accel_y,accel_z;
        
        /** \brief Angular Velocity as received from OSCulator ,this variable is helpful to predict if the swing is going forwards or backward .A +ve value means the swing is going front and a -ve value indicates the person is swinging backwards */
        float angular_velocity;
        
        /** \brief OSCReceiver class that interfaces with the OSCulator and receives values from the wii-mote */
        ofxOscReceiver receiver;
        
        /** \brief The message received from OSCulator containing details of the wiimote values like angular velocity and acceleration */
        string Message;
        
        int windowWidth,windowHeight;
    
        /** Unused with current version but could be used if you want to make the camera position go front and back with wii-mote.See code for further details */
        float prevAngVel=0;
    
        /** @} */
    
        
    #ifdef ADJUSTTIMEGAP
      
        /** \brief These are the wii-mote variables that are used to adjust the timegap between two images */
        float minAngularVelocity,maxAngularVelocity;
        float minAccel,maxAccel;
        
    #endif
        /** \brief Font to display the wiimote values on screen for debugging*/
        ofTrueTypeFont fonttodisplayWiimoteValues;
    
        /** \brief Swing's current state i.e  */
        string State;
    #endif
        
        /** \brief Time that an image will be shown on the screen,this is controlled by the wii-motes acceleration */
        float timeGap;
        
        
        /** \brief This variable is used to check if a half-swing is completed ,that is from back to front or from front to back */
        bool isturnCompleted;
        
        /** \brief If set,this variable will cause the currentVolume variable to fade */
        bool fadeAudio;
        
        /** \brief The current volume variable is generally set to 1 ,but if fadeAudio is enabled,it will reduce the audio by -0.001 every frame,this is used in the end along with the Corresponding video to reduce the volume of the music */
        float currentVolume;

        /** \brief This variable is used to start the installation if the user is swinging on the wii-mote or if the enter key is pressed  */
        bool startInstallation;
          
        /* 
         * The below variables are used in the starting if the person is not swinging 
         */
        
        /** \brief previewText is used for loading the font Asyouwish.ttf */
        ofTrueTypeFont previewText;
        
        /** \brief This string will display "Start Swinging !" at the start */
        string tempText;
        
        /** \brief If true,the ending variable will trigger the backward playing of the starting video and provides a nice smooth ending */
        bool ending;
                
        /// Unused 
                
        /** \brief Datastructure used to store details and images of the user's tagged images */
        vector<ImageData> taggedImageObjects;
        
    
        /** @addtogroup CameraAnimation
         *
         * @{
         Vector that will use the above vector and ofLerp to compute the exact cameraposition
         @returns ofVec3f
         */
        ofVec3f adjustoverShotCameraPosition();
        

        /**
         Given 2 indices (that are positions in a conical helix,this Function will animate the camera from one position to another using ofLerp
         @param pos1 is the position from which we will animateFrom,pos2 
         @returns ofVec3f
         */
        ofVec3f animate(int,int);
        
        /**
         This function plays the starting animation which zooms out of the spiral
        */
        void startAnimation();

        /** If the starting animation is active,this function will return a vector that has the startingCamera's intermediate tweened position
         @returns ofVec3f*/
        ofVec3f startAnimationCameraPosition();
    
        /** @} */

        /** @addtogroup Wiggle
         * @{
         *
         */
    
        /** Pushing values */
        void pushWigglePositions();
    
        /**
         Actual code that does wiggling.
         @returns ofVec3f
         */
        ofVec3f wiggle();
    
        /** @} */
    
        /** Load the Images and XML Data.This function will load the images from the directory and then parse the xml to get the score and album number.Once that is done ,we combine the untagged and tagged images to get a combined Data Structure  */
        void loadImagesandXMLData();
    
        
        /** This function will draw the images.Used in draw */
        void drawImages();
        
        /** Function that will generate the points on the Spiral */
        void generateCircularSpiral();
           
        /** @addtogroup DrawingStars
          *
         * Drawing small dot stars in the background was attempted.Unfortunately,its too memory intensive ,nevertheless these functions are included if anyone is interested in trying them out.Just call assignStarPositions() in the setup function and drawStars() in the draw function 
         * @{
         */
        void assignStarPositions();
        void drawStars();
    
        /** @} */
        
    
};
