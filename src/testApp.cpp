#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
    ofHideCursor();
    ofBackground(0, 0, 0);
    
    generateCircularSpiral();
    loadImagesandXMLData();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(200);
    
#ifdef DEBUGMODE
    camera.setDistance(200);
    camera.setFarClip(1000000);
#else
    
    camera.setFov(40);
    
    /// Setting a very large far clip
    camera.setFarClip(10000000);
    cameraindex=combinedImageObjects.size()-1;
    
    camera.setPosition(cameraStartPosition);
    animationMode=false;
    isstartingAnimationActive=true;
    
    startoverShotCameraAnimation=false;
    
    overshotCameraStartingPosition=ofVec3f(0,0,0);
    startAnimationCounter=0;
    pushWigglePositions();
    currentwiggleindex=0;
    wiggleAnimationCounter=0;
    startInstallation=false;
    
#endif
    
    ofEnableAlphaBlending();
    
    ofSetBackgroundAuto(true);
    
    numberofImages=combinedImageObjects.size()-1;
    
    
#ifdef USEWII
    
    // For debug purposes
    //cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup( PORT );
    
    accel_x=accel_y=accel_z;
    
    Message="";
     accel=0;
    
#ifdef ADJUSTTIMEGAP
    minAngularVelocity=10000;
    maxAngularVelocity=(-1000);
    minAccel=1000;maxAccel=-1000;
#endif
    
    State="";
    fonttodisplayWiimoteValues.loadFont("Inconsolata.otf", 20);
    
#endif
    
    BluementhalMp3.loadSound("Blumenthal.mp3");
    BluementhalMp3.setVolume(1.0f);
    
    timeGap=6000; // This is the default timeGap .Can be easily changed.
    
    ofSetFullscreen(true);
    currentVolume=1;
    fadeAudio=false;
    
    startingMovieFinished=false;
    startingMovie.loadMovie("Intro_new.mov", OF_QTKIT_DECODE_TEXTURE_ONLY);
    startingMovie.setLoopState(OF_LOOP_NONE);
    
    cout<<startingMovie.getDuration()<<endl;
    
    cout<<startingMovie.getWidth()<<"    "<<startingMovie.getHeight();
    
    previewText.loadFont("asyouwish.ttf",42);
    ending=false;
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Update the frame
    startingMovie.update();
    
    if(startingMovie.getIsMovieDone())
        startingMovieFinished=true;
    
    if(startingMovie.getCurrentFrame()==0&&ending)
    {cout<<"ending"<<endl;
        ofExit();
    }
    ofSoundUpdate();
    
#ifdef USEWII
    
    // Check if there are any pending messages to be received from OSCulator 
    
    while( receiver.hasWaitingMessages())
    {
        if(windowWidth == 0 || windowHeight == 0){
            windowWidth = ofGetWidth();
            windowHeight = ofGetHeight();
        }
        
    // Get the next message
        ofxOscMessage m;
        float x,y;
        receiver.getNextMessage( &m );
        
    // For Debugging Purposes
    // cout<<m.getArgAsFloat( 0 )<<endl;
        
        if ( m.getAddress() == "/wii/2/ir/0" )
        {
            x = m.getArgAsFloat( 0 );
            
            wiiX = x * windowWidth;
            cout << "x: " << wiiX << " y: " << wiiY << "\n";
        }
        else if ( m.getAddress() == "/wii/2/ir/1" )
        {
            y = 1 - m.getArgAsFloat( 0 );
            wiiY = y * windowHeight;
            cout << "x: " << wiiX << " y: " << wiiY << "\n";
        }
        else if (m.getAddress() == "/wii/1/accel/pry/1") {
            roll = m.getArgAsFloat(0);
            
        } else if (m.getAddress() == "/wii/1/accel/pry/2") {
            yaw = m.getArgAsFloat(0);
        }
        else if (m.getAddress() == "/wii/1/accel/pry/0") {
            pitch = m.getArgAsFloat(0);
            
        } else if (m.getAddress() == "/wii/1/accel/pry/3") {
            accel = m.getArgAsFloat(0);
        }
        
        else if(m.getAddress()=="/wii/1/accel/xyz/0")
        {
            accel_x=m.getArgAsFloat(0);
        }
        
        else if(m.getAddress()=="/wii/1/accel/xyz/1")
        {
            accel_y=m.getArgAsFloat(0);
        }
        
        else if(m.getAddress()=="/wii/1/accel/xyz/2")
        {
            accel_z=m.getArgAsFloat(0);
        }
        
        else if(m.getAddress()=="/wii/1/motion/velo/0")
            angular_velocity=m.getArgAsFloat(0);
        
        else
        {
#ifdef DEBUG
            cout << "unrecognized message: " << m.getAddress() << "\n";
#endif
        }
    }
    
    if(abs(angular_velocity*1000)<400)
        prevAngVel=angular_velocity;
    
    if(abs(accel*1000)-200>2&&!startInstallation)
    {
        startInstallation=true;
        BluementhalMp3.play();
        startingMovie.play();
    }
    cout<<"Acceleration :"<<accel<<endl;
#endif
    
#ifndef USEWII
    if(startInstallation)
    {
    //BluementhalMp3.play();
    startingMovie.play();
}
#endif


    if(fadeAudio)
        currentVolume-=0.001;
    
    BluementhalMp3.setVolume(currentVolume);
}

//--------------------------------------------------------------
void testApp::draw()
{
    
    // Only if the wii-mote accelerates a bit i.e the swing moves or if the user presses "Enter"

    if(startInstallation)
    {
        // Play the Movie first
        
        if(!startingMovieFinished)
        {
            startingMovie.draw(0, 0);
        }
        
        else
        {
#ifndef DEBUGMODE
            ofBackground(0, 0, 0);
            ofSetColor(255,255,255);
            
            // If startinganimation is active,have the startingCameraAnimation or the overshotCameraPosition
            if(isstartingAnimationActive)
            {
                if(!startoverShotCameraAnimation)
                    camera.setPosition(startAnimationCameraPosition());
                else camera.setPosition(adjustoverShotCameraPosition());
                
                timesinceLastTransition=ofGetElapsedTimeMillis();
                //animationMode=true;
            }
            
            else
            {
                if((ofGetElapsedTimeMillis()-timesinceLastTransition)>timeGap&&cameraindex!=0&&!animationMode)
                {
                 
                    cameraindex--;
                    
                    //        cout<<"The current camera index value is "<<imageData[cameraindex].y<<endl;
                    animationMode=true;
                    animationCounter=0;
                    timesinceLastTransition=ofGetElapsedTimeMillis();
                    
                }
                
                // If we are at the last image,or if the song is almost towards the end . End the experience by playing the startingMovie in reverse.
                else if(cameraindex==0||BluementhalMp3.getPosition()>0.92)
                {
                    startingMovieFinished=false;
                    ending=true;
                    startingMovie.setSpeed(-1); // Reverse the video being played
                    startingMovie.play();
                    fadeAudio=true;
                    
                }
                
                if(animationMode)
                {
                    // Animate the camera from one index to another ..
                    if(combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight()>=combinedImageObjects[numberofImages-cameraindex].theloadedimage.getWidth())
                        camera.setPosition(animate(cameraindex+1, cameraindex));
                    else camera.setPosition(animate(cameraindex+1, cameraindex));
                }
                
                else
                {
                    // Depending on whether the image is more wide or tall,we set the camera's position to be at a fixed position from the image and also wiggle the position of the camera.
                    
                    if(combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight()>=combinedImageObjects[numberofImages-cameraindex].theloadedimage.getWidth())
                    {
                        camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,1.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight())+wiggle());

                    }
                    else camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,1.05*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getWidth())+wiggle());
                    
                }
                
            }
            
#endif
            
            camera.begin();
            
            ofPushMatrix();
            ofRotateX(180);
            ofPopMatrix();
            drawImages();
                        
            camera.end();
            
        }
        
#ifdef USEWII
        
        // If acceleration is greater than a particular value ,Adjust the timegap or see if it is going backward or forward 
        
        if(accel-0.2>0.008&&accel!=0)
        {
            if(angular_velocity<=-0.03)
            {
                ofSetColor(255, 0, 0);
                
                if(State.compare("Front")==0)
                    isturnCompleted=true;
                
                State="Back";
                
            }
            
            else if(angular_velocity>=0.03)
            {
                
                ofSetColor(0,255, 0);
                // fonttodisplayWiimoteValues.drawString("Front", ofGetWidth()/2,ofGetHeight()/2+50);
                
                if(State.compare("Back")==0)
                    isturnCompleted=true;
                
                State="Front";
            }
            
#ifdef ADJUSTTIMEGAP
            
            if(angular_velocity<minAngularVelocity)
                minAngularVelocity=angular_velocity;
            if(angular_velocity>maxAngularVelocity)
                maxAngularVelocity=angular_velocity;
            
            if(accel<minAccel)
                minAccel=accel;
            if(accel>maxAccel&&accel*1000<400)
                maxAccel=accel;
            
            if(isturnCompleted)
            {
                
                // Adjust the time gap between 2 images.Depending upon the acceleration we adjust the timegap between 2 which 2 images are seen .
                
                maxAccel*=1000;
                if(maxAccel>204&&maxAccel<=214)
                    timeGap=9000;
                if(maxAccel>214&&maxAccel<=224)
                    timeGap=7500;
                if(maxAccel>224&&maxAccel<=234)
                    timeGap=6000;
                if(maxAccel>234&&maxAccel<=244)
                    timeGap=5000;
                if(maxAccel>244&&maxAccel<=254)
                    timeGap=4000;
                
                
                maxAccel=0;
                isturnCompleted=false;
            }
            //        fonttodisplayWiimoteValues.drawString(ofToString(timeGap), ofGetWidth()/2+200, ofGetHeight()/2+50);
            
            
#endif
            
        }
        
        ofSetColor(255, 255, 255);
        
        /* Use these to get the Acceleration and Velocity values from the Wii-mote*/
        
        //    fonttodisplayWiimoteValues.drawString("Acceleration "+ofToString(accel*1000), ofGetWidth()/2, ofGetHeight()/2+100);
        //    fonttodisplayWiimoteValues.drawString("Velocity "+ofToString(angular_velocity*1000), ofGetWidth()/2, ofGetHeight()/2+200);
        //   fonttodisplayWiimoteValues.drawString("Max Acceleration "+ofToString(maxAccel*1000), ofGetWidth()/2, ofGetHeight()/2+250);
        //    fonttodisplayWiimoteValues.drawString("Max Velocity "+ofToString(max*1000), ofGetWidth()/2, ofGetHeight()/2+350);
    
    //     cout<<"Acceleration "<<accel*1000   <<"\n Max Acceleration "<<maxAccel*1000<<"\n\n Min Acceleration "<<minAccel<<endl;
    //
    //    cout<<"\nAngular Velocity "<<angular_velocity<<"\n Max Velocity "<<max*1000<<"\n\n Min Velocity "<<minAngularVelocity<<endl;;
    
    
#endif
        
}
    else{
        
        startingMovie.draw(0, 0);
        
        ofSetColor(255, 255, 255);
        
        tempText="Start Swinging !";
        
        previewText.drawString(tempText, 500, startingMovie.getHeight()-25);
        
        
        
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key=='f'||key=='F')
    {
        ofToggleFullscreen();
    }
    
#ifndef DEBUGMODE
    
    else if(key==OF_KEY_UP&&cameraindex!=numberofImages&&!animationMode)
    {
        cameraindex++;
        animationMode=true;
        
    }
   
    else if(key==OF_KEY_DOWN&&cameraindex!=0&&!animationMode&&!isstartingAnimationActive&&!startoverShotCameraAnimation)
    {
        cameraindex--;
        //        cout<<"The current camera index value is "<<imageData[cameraindex].y<<endl;
        animationMode=true;
        animationCounter=0;
        
    }
    else if(key==OF_KEY_RETURN&&!startInstallation)
    {
        startInstallation=true;
        BluementhalMp3.play();
        startingMovie.play();
    }
    
     cout <<startingMovieFinished<<"     "<<startoverShotCameraAnimation;
#endif
        
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

void testApp::generateCircularSpiral()
{
    int spreadDistance=30;
    
    
    /* Conical Concentric Circles */
    
    //    for(int r=0;r<200;r+=spreadDistance)
    //    {
    //        for(float angle=0;angle<=360;angle+=4)
    //            SpiralPoints.push_back(ofVec3f(r*cos(ofDegToRad(angle)),r*sin(ofDegToRad(angle)),r));
    //    }
    
    // Generating a Cylindrical Helix Structure ,equation adapted from http://www.mathematische-basteleien.de/spiral.htm .Few Tweaks have been made to get the right structure
    
    
    float height=10000,radius=200,ang_freq=3.2;
    
    
    for(float angle=0;angle<=3600;angle+=0.01)
    {
        
        float angle2=angle;
        
        SpiralPoints.push_back(ofVec3f(((height-angle2)/height)*radius*cos(ofDegToRad(ang_freq*angle2)),((height-angle2)/height)*radius*sin(ofDegToRad(ang_freq*angle2)),6*angle2));
        
    }
    
#ifndef DEBUGMODE
    cameraStartPosition=SpiralPoints.back();
    cameraEndPosition=SpiralPoints.front();
#endif
    
}

void testApp::drawImages()
{
    
    std::multimap<int,int>::iterator it;
    int imageIterator=0;
    
    
    for(int i=numberofImages-1;i>=0;i--) // Draw images in reverse order ....
    {
        
        ofPushMatrix();
        
        ofTranslate(35*SpiralPoints[700*(combinedImageObjects.size()-1-i)].x,35*SpiralPoints[700*(combinedImageObjects.size()-1-i)].y,35*SpiralPoints[700*(combinedImageObjects.size()-1-i)].z); // 35 gives good results .You can use a higher number for more spread ,but you have to change this number throughout this file.

        combinedImageObjects[i].theloadedimage.draw(-combinedImageObjects[i].theloadedimage.getWidth()/2,-combinedImageObjects[i].theloadedimage.getHeight()/2);
        
        ofPopMatrix();
        
    }
}

#ifndef DEBUGMODE

// This function will animate the camera from one point in the spiral to another point 

ofVec3f testApp::animate(int pos1, int pos2)
{
    
    float smoothnessFactor=2400,timeInterval=10;
    
    if(animationCounter<=smoothnessFactor-timeInterval)
    { tweenvalue = (animationCounter) /smoothnessFactor;
        animationCounter+=timeInterval;
    }
    else {
        tweenvalue=0;
        animationMode=false;
        
        if(combinedImageObjects[numberofImages-pos2].theloadedimage.getHeight()>=combinedImageObjects[numberofImages-pos2].theloadedimage.getWidth())
            return ofVec3f(35*SpiralPoints[700*pos2]+ofVec3f(0,0,1.6*combinedImageObjects[numberofImages-pos2].theloadedimage.getHeight())+wiggle());
        
        else return ofVec3f(35*SpiralPoints[700*pos2]+ofVec3f(0,0,1.05*combinedImageObjects[numberofImages-pos2].theloadedimage.getWidth())+wiggle());
        
    }
    tweenedCameraPosition.x=ofLerp(35*SpiralPoints[700*pos1].x,35*SpiralPoints[700*pos2].x,tweenvalue);
    tweenedCameraPosition.y=ofLerp(35*SpiralPoints[700*pos1].y,35*SpiralPoints[700*pos2].y,tweenvalue);
    
    
    // Setting the Z value ..
    
    if(combinedImageObjects[numberofImages-pos1].theloadedimage.getHeight()>=combinedImageObjects[numberofImages-pos1].theloadedimage.getWidth())
        position1_z=1.6*combinedImageObjects[numberofImages-pos1].theloadedimage.getHeight()+35*SpiralPoints[700*pos1].z;
    else position1_z=1.05*combinedImageObjects[numberofImages-pos1].theloadedimage.getWidth()+35*SpiralPoints[700*pos1].z;
    
    if(combinedImageObjects[numberofImages-pos2].theloadedimage.getHeight()>=combinedImageObjects[numberofImages-pos2].theloadedimage.getWidth())
        position2_z=1.6*combinedImageObjects[numberofImages-pos2].theloadedimage.getHeight()+35*SpiralPoints[700*pos2].z;
    else position2_z=1.05*combinedImageObjects[numberofImages-pos2].theloadedimage.getWidth()+35*SpiralPoints[700*pos2].z;
    
    
    tweenedCameraPosition.z=ofLerp(position1_z,position2_z,tweenvalue);
    
    return tweenedCameraPosition;
    
}

ofVec3f testApp::startAnimationCameraPosition()
{
    
    float smoothnessFactor=35*SpiralPoints[700*cameraindex].z +1.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight();
    
    float timeInterval=smoothnessFactor/1500;
    
    if(startAnimationCounter<=smoothnessFactor-timeInterval)
    {
        tweenvalue = (startAnimationCounter) /smoothnessFactor;
        
        if(tweenvalue<0.98)
            startAnimationCounter+=timeInterval;
        else startAnimationCounter+=(timeInterval);
        
        overshotCameraStartingPosition=tweenedCameraPosition;
    }
    
    //else isstartingAnimationActive=false;
    else {startAnimationCounter=0; startoverShotCameraAnimation=true;
    }
    
    tweenedCameraPosition.x=ofLerp(0, 0 , tweenvalue);
    tweenedCameraPosition.y=ofLerp(0, 0, tweenvalue);
    tweenedCameraPosition.z=ofLerp(0, 35*SpiralPoints[700*cameraindex].z +21.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight(), tweenvalue);
    
    return tweenedCameraPosition;
    
}

#endif

testApp::testApp(long long int id)
{
    userid=id;
    cout<<"\n User ID:"<<id<<endl;
}

void testApp::pushWigglePositions()
{
    
    // wigglePositions.push_back(ofVec3f(0,0,0));
    
    wigglePositions.push_back(ofVec3f(1,1,0));
    wigglePositions.push_back(ofVec3f(1,1,0));
    wigglePositions.push_back(ofVec3f(0,1,0));
    wigglePositions.push_back(ofVec3f(0,1,0));
    wigglePositions.push_back(ofVec3f(1,0,0));
    wigglePositions.push_back(ofVec3f(1,0,0));
    
}

ofVec3f testApp::wiggle()
{
    
    int maxValue=20; // Change this for adjusting the maximum Value of Wiggling .
    
    // For every even value of currentwiggleindex,we move to the right, or down or bottom right. and move in left,up or top left for every odd index .
    // To change the wiggle motion Just change the wigglePositions points.	
    
    if(currentwiggleindex%2==0)
    {
        
        if(wiggleAnimationCounter<=maxValue) 
            wiggleAnimationCounter+=0.05; // Increase the wiggleAnimationCounter till it reaches 20 
        
        else { // Once it reaches 20,advance the wiggleindex,The Image will wiggle to another direction 
            currentwiggleindex++;
            currentwiggleindex=currentwiggleindex%wigglePositions.size();
            wiggleAnimationCounter=maxValue;
        }
    }
    
    else
    {
        
        if(wiggleAnimationCounter>=0)
            wiggleAnimationCounter-=0.05 ;
        
        else {
            currentwiggleindex++;
            currentwiggleindex=currentwiggleindex%wigglePositions.size();
            wiggleAnimationCounter=0;
        }
    }
    
    
    currentwigglePosition.x=wigglePositions[currentwiggleindex].x*wiggleAnimationCounter;
    currentwigglePosition.y=wigglePositions[currentwiggleindex].y*wiggleAnimationCounter;
    currentwigglePosition.z=wigglePositions[currentwiggleindex].z*wiggleAnimationCounter;
    
    return currentwigglePosition;
}

void testApp::drawStars()
{
    ofSetColor(255, 255, 255);
    
    for(int i=0;i<StarPositions.size();i++)
        ofSphere(StarPositions[i], 1);
    
}

void testApp::assignStarPositions()
{
    for(int i=0;i<10000;i++)
        StarPositions.push_back(ofVec3f(ofRandom(100000), ofRandom(10000),ofRandom(10000)));
    
}


void testApp::loadImagesandXMLData()
{
    // Change this to your Directory Path 
    string untaggedDirpath=PathToImageFolder+ofToString(userid)+"/untaggedImages/";
    
    ofDirectory untaggeddir(untaggedDirpath);
    untaggeddir.allowExt("png");
    untaggeddir.allowExt("jpg");
    untaggeddir.allowExt("gif");
    
    if(untaggeddir.listDir()==0)
    {
    
    // Debug Statements 
    cout<<"Nothing";   ofExit();
    }
    
    cout<<untaggeddir.listDir();
    cout<<"NUMBER OF FILES"<<untaggeddir.numFiles()<<endl;
    
    ofImage TempImage;
    
    int imageCounter=0;
    
    string xmlpath=PathToImageFolder+ofToString(userid)+"/imagedata.xml";
    
    // Just a temporary ImageData object 
    ImageData imagedataObject;
    
    // Parse the XML,Load all the UntaggedImages into the combinedImageObjects data structure .These images will be already sorted in descending order of their scores .
    
    if(pictures_XML.loadFile(xmlpath))
    {
        
        pictures_XML.pushTag("xml");
        pictures_XML.pushTag("ImageList");
        
        if(pictures_XML.pushTag("Untagged"))
        {
            
            for (int j=0;j<pictures_XML.getNumTags("Image");j++)
            {
                pictures_XML.pushTag("Image",j);
                
                imagedataObject.imageNumber=imageCounter;
                imagedataObject.imageScore=pictures_XML.getValue("Score",0.0);
                imagedataObject.albumnumber=pictures_XML.getValue("AlbumNumber",0);
                imagedataObject.theloadedimage.loadImage(ofToString(untaggedDirpath+ofToString(j)+".jpg"));
                
                imagedataObject.theloadedimage.resize(imagedataObject.theloadedimage.getWidth(), imagedataObject.theloadedimage.getHeight());
                
#ifndef BLUR
               
                imagedataObject.theloadedimage.mirror(true,false);
#endif
          
                combinedImageObjects.push_back(imagedataObject);
                
                pictures_XML.popTag();
                
                imageCounter++;
                
            }
            pictures_XML.popTag();    
        }
    }
    
    // Debug statement 
    
    cout<<combinedImageObjects.size()<<"\t this was the size of untaggedIamgeobjects";
    
    ImageVector.clear();
    imageCounter=0;
    
    
    
    
    string taggedDirpath=PathToImageFolder+ofToString(userid)+"/taggedImages/";
    ofDirectory taggeddir(taggedDirpath);
    
    taggeddir.allowExt("png");
    taggeddir.allowExt("jpg");
    taggeddir.allowExt("gif");
    
    // Now all the Tagged images need to be linked and added to the data structure .
        
    if(pictures_XML.pushTag("Tagged"))
        
    {
        for (int j=0;j<pictures_XML.getNumTags("Image");j++)
        {
            pictures_XML.pushTag("Image",j);
            
            imagedataObject.imageNumber=imageCounter;
            imagedataObject.imageScore=pictures_XML.getValue("Score",0.0);
            imagedataObject.albumnumber=pictures_XML.getValue("AlbumNumber",0);
            imagedataObject.theloadedimage.loadImage(ofToString(taggedDirpath+ofToString(j)+".jpg"));
#ifndef BLUR
            imagedataObject.theloadedimage.mirror(true,false);
#endif
            
            taggedImageObjects.push_back(imagedataObject);
            
            pictures_XML.popTag();
            
            imageCounter++;
            
        }
    }
    
    
    cout<<taggedImageObjects.size()<<"\t this was the size of taggedImgeobjects";
    
    // Now combine the untagged and Tagged in the correct ratio ..
    
    int ratio= 3; // This ratio defines how many images should be taken from the untagged Images compared to the TaggedImages .For every 2 untaggedimages,1 taggedimage is displayed.This usually strikes a nice balance based on our experience with 50 + users.However,it could be modified .    
    
    int untaggedImageCount=0,taggedImageCount=0,i;
    int num_untaggedImages=combinedImageObjects.size(),num_taggedImages=taggedImageObjects.size();
    int taggedImgCount=0;
    
    cout<<"Combining and shuffling the tagged and untagged images";
    
    // The below loop shuffles the images so that the structure based on the defined ratio
    
    for(int i=0;i<combinedImageObjects.size();i++)
    {
        if(i%ratio==0&&i!=0&&taggedImgCount<num_taggedImages)
            combinedImageObjects.insert(combinedImageObjects.begin()+i,taggedImageObjects[taggedImgCount++]);
        
    }
    
    //  Parse the data structure again,to make sure that no 2 images having the same albumnumber are next to each other.This is done so that different images are shown.This can be experimented with.Images from alternate albums have worked for us.
    
    int j,tempVal;
    
    for(int i=0;i<combinedImageObjects.size()-1;i++)
    {
        j=i+1;
        cout<<i;
        
        if(combinedImageObjects[i].albumnumber==combinedImageObjects[j].albumnumber)
        {
            cout<<"doing this when i= \t "<<combinedImageObjects[i].albumnumber<<"\t";
            
            while(j<combinedImageObjects.size()-1&&(combinedImageObjects[i].albumnumber==combinedImageObjects[j].albumnumber) && (j-i)<8) // This is because j is
            {
                j++;
            }
            cout<<" and then j= \t"<<combinedImageObjects[j].albumnumber<<endl;
            
            std::swap(combinedImageObjects[i+1], combinedImageObjects[j]);
            
        }
    }
    
    cout<<"Now checking the album numbers ,Size of the array = \t"<<combinedImageObjects.size()<<endl;
    
    for(int i=0;i<combinedImageObjects.size();i++)
        cout<<"Album number:"<<combinedImageObjects[i].albumnumber<<"\t Score"<<combinedImageObjects[i].imageScore<<endl;
       
}

/* This function will bring the cameraposition to the starting image once it has 'overshot' while zooming out of the spiral */

ofVec3f testApp::adjustoverShotCameraPosition()
{
    
    float smoothnessFactor=35*SpiralPoints[700*cameraindex].z +1.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight();

    float timeInterval=smoothnessFactor/500;
    
    //    cout<<"smoothness factor\n"<<smoothnessFactor;
    
    if(startAnimationCounter<=smoothnessFactor-timeInterval)
    {
        
        cout<<"\nCamera Position"<< tweenedCameraPosition.z;
        
        tweenvalue = (startAnimationCounter) /smoothnessFactor;
        
        if(tweenvalue<0.98)
            startAnimationCounter+=timeInterval;
        else startAnimationCounter+=(timeInterval);
        
    }
    
    else {isstartingAnimationActive=false;startoverShotCameraAnimation=false;}
    
    tweenedCameraPosition.x=ofLerp(0, 35*SpiralPoints[700*cameraindex].x, tweenvalue);
    tweenedCameraPosition.y=ofLerp(0, 35*SpiralPoints[700*cameraindex].y, tweenvalue);
    tweenedCameraPosition.z=ofLerp(overshotCameraStartingPosition.z, 35*SpiralPoints[700*cameraindex].z +1.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight(), tweenvalue);
    
    return tweenedCameraPosition;
    
}
