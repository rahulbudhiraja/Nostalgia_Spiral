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
    camera.setFarClip(10000000);
    cameraindex=combinedImageObjects.size()-1;
    
    accel=0;
    
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

#ifdef BLUR
    blur.setup(ofGetScreenWidth(), ofGetScreenHeight(), 1, .05, 10);
#endif
    #endif
    
    ofEnableAlphaBlending();
  
    ofSetBackgroundAuto(true);
    
    numberofImages=combinedImageObjects.size()-1;
    
    
#ifdef USEWII
    
    // Wii-mote Variables ..
    
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup( PORT );
    
    accel_x=accel_y=accel_z;
    
    Message="";
    
    // End of the wii-mote Variables .....
    
    
#ifdef ADJUSTTIMEGAP
    minAngularVelocity=10000;
    maxAngularVelocity=(-1000);
    minAccel=1000;maxAccel=-1000;
#endif
    
    State="";
    font.loadFont("Inconsolata.otf", 20);
    
#endif
 
    BluementhalMp3.loadSound("Blumenthal.flac");
    BluementhalMp3.setVolume(1.0f);
    
    timeGap=6000;
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
    startingMovie.update();

    if(startingMovie.getIsMovieDone())
        startingMovieFinished=true;
    
    if(startingMovie.getCurrentFrame()==0&&ending)
    {cout<<"ending"<<endl;
        ofExit();
    }
    ofSoundUpdate();
    
#ifdef USEWII
    
    while( receiver.hasWaitingMessages())
    {
        if(windowWidth == 0 || windowHeight == 0){
            windowWidth = ofGetWidth();
            windowHeight = ofGetHeight();
        }
        // get the next message
        ofxOscMessage m;
        float x,y;
        receiver.getNextMessage( &m );
        cout<<m.getArgAsFloat( 0 )<<endl;
        
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
        
    if(fadeAudio)
        currentVolume-=0.001;
    
    BluementhalMp3.setVolume(currentVolume);
}

//--------------------------------------------------------------
void testApp::draw()
{
  
if(startInstallation)
{
    
    if(!startingMovieFinished)
    {
     startingMovie.draw(0, 0);

    }
    else
    {
#ifndef DEBUGMODE
      ofBackground(0, 0, 0);
ofSetColor(255,255,255);
    if(isstartingAnimationActive)
    {
        if(!startoverShotCameraAnimation)
        camera.setPosition(startAnimationCameraPosition());
        else camera.setPosition(adjustoverShotCameraPosition());
        timesinceLastTransition=ofGetElapsedTimeMillis();
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
        
        else if(cameraindex==0||BluementhalMp3.getPosition()>0.92)
        {
            startingMovieFinished=false;
            ending=true;
            startingMovie.setSpeed(-1);
            startingMovie.play();
            fadeAudio=true;
            
        }
//        cout<<"The dimensions of the Image are: width = "<<ImageVector[imageDetails[cameraindex].imageNumber-1].getWidth()<<" \t Height = "<<ImageVector[imageDetails[cameraindex].imageNumber-1].getHeight()<<"Max value is\t"<<max(ImageVector[imageDetails[cameraindex].imageNumber-1].getHeight(),ImageVector[imageDetails[cameraindex].imageNumber-1].getWidth())<<"Index is"<<imageDetails[cameraindex].imageNumber<<endl;;
//
//
//        cout<<"\nThe image score"<<imageDetails[cameraindex].imageScore;
        
    if(animationMode)
    {
        if(combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight()>=combinedImageObjects[numberofImages-cameraindex].theloadedimage.getWidth())
            camera.setPosition(animate(cameraindex+1, cameraindex));
        else camera.setPosition(animate(cameraindex+1, cameraindex));
          
    
        
    }
    else
        

    {
        
       if(combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight()>=combinedImageObjects[numberofImages-cameraindex].theloadedimage.getWidth())
        {
         camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,1.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight())+wiggle());
//            cout<<ImageVector[imageDetails[cameraindex].imageNumber-1].getHeight()/tan(ofDegToRad(20))<<"\t";
        }
        else camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,1.05*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getWidth())+wiggle());
        
//        camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,1.05*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getWidth())+wiggle()+ofVec3f(0,0,prevAngVel*150)); // This is for the wii-mote
        
//        else camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,1000)-wiggle()); // So that the camera goes backward ..
        
    }
        
        
        
    }
    
#endif
    

    camera.begin();

    ofPushMatrix();
    ofRotateX(180);
    ofPopMatrix();
    drawImages();

    
    camera.end();
#ifdef BLUR
    blur.draw();
#endif

}

#ifdef USEWII
    
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
//            font.drawString("Front", ofGetWidth()/2,ofGetHeight()/2+50);
            
            if(State.compare("Back")==0)
                isturnCompleted=true;
            
            State="Front";
        }
        
#ifdef USEWII
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


          // Adjust the time gap ...
          
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
//        font.drawString(ofToString(timeGap), ofGetWidth()/2+200, ofGetHeight()/2+50);
        
        
#endif
        
        
#endif
    }
    
    ofSetColor(255, 255, 255);
    
/* Use these to get the Acceleration and Velocity values from the Wii-mote*/
    
//    font.drawString("Acceleration "+ofToString(accel*1000), ofGetWidth()/2, ofGetHeight()/2+100);
//    font.drawString("Velocity "+ofToString(angular_velocity*1000), ofGetWidth()/2, ofGetHeight()/2+200);
//    font.drawString("Max Acceleration "+ofToString(maxAccel*1000), ofGetWidth()/2, ofGetHeight()/2+250);
//    font.drawString("Max Velocity "+ofToString(max*1000), ofGetWidth()/2, ofGetHeight()/2+350);
}
//     cout<<"Acceleration "<<accel*1000   <<"\n Max Acceleration "<<maxAccel*1000<<"\n\n Min Acceleration "<<minAccel<<endl;
//    
//    cout<<"\nAngular Velocity "<<angular_velocity<<"\n Max Velocity "<<max*1000<<"\n\n Min Velocity "<<minAngularVelocity<<endl;;
    
    
#endif
    
else {
    
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
    else if(key==OF_KEY_DOWN&&cameraindex!=0&&!animationMode)
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
    
    else
    {
#ifdef BLUR
        ofImage imgSaver;
        ofFbo tp=blur.returnbase();
        tp.readToPixels(imgSaver.getPixelsRef());
        
        imgSaver.update();
        
        ofSaveImage(imgSaver,"blah.png",OF_IMAGE_QUALITY_BEST);
#endif
    }
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


void testApp::loadImagesFromDirectory()
{
    
    string path="/Users/rahulbudhiraja/Work/of_v0073_osx_release/apps/myApps/Nostalgia_Spiral/bin/data/Images/"+ofToString(userid)+"/"; // Change this path depending upon your directory 
    
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.allowExt("gif");
    
    cout<<dir.listDir();
    
    cout<<"NUMBER OF FILES"<<dir.numFiles()<<endl; // Debug Statement .
    
    ofImage TempImage;
    
    for(int i = 0; i < dir.numFiles(); i++){
        if(!TempImage.loadImage(ofToString(path+ofToString(i+1)+".jpg")))
            continue; // OSX indexing .. and i+1 is because there is nothing called 0.jpg .....
        TempImage.resize(TempImage.getWidth(), TempImage.getHeight());
        
#ifndef BLUR
        
      
        TempImage.mirror(true,false);
#endif

        ImageVector.push_back(TempImage);

        // cout<<TempImage.getWidth()<<"\t\t"<<TempImage.getHeight()<<"\n\n";
        TempImage.clear();


    }
   
}



void testApp::drawImages()
{
    
    std::multimap<int,int>::iterator it;
    int imageIterator=0;
    
    
        for(int i=numberofImages-1;i>=0;i--) // Draw images in reverse order ....
        {
    
        ofPushMatrix();

#ifdef BLUR
            blur.begin();
#endif
        ofTranslate(35*SpiralPoints[700*(combinedImageObjects.size()-1-i)].x,35*SpiralPoints[700*(combinedImageObjects.size()-1-i)].y,35*SpiralPoints[700*(combinedImageObjects.size()-1-i)].z);
         
        ; //cout<<35*SpiralPoints[700*i]<<"\t";
        
            
      
        combinedImageObjects[i].theloadedimage.draw(-combinedImageObjects[i].theloadedimage.getWidth()/2,-combinedImageObjects[i].theloadedimage.getHeight()/2);
#ifdef BLUR
            blur.end();
#endif
        ofPopMatrix();
        
         
//        imageIterator++;
        //if(i>=1)
            ;// ofLine(35*SpiralPoints[700*i],35*SpiralPoints[700*(i-1)]);
    }
}

#ifndef DEBUGMODE

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

//        return ofVec3f(35*SpiralPoints[700*pos2].x,35*SpiralPoints[700*pos2].y,35*SpiralPoints[700*pos2].z);
        
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
    
    
//     cout<<tweenvalue<<"\n";
    
    return tweenedCameraPosition;
    
}

ofVec3f testApp::startAnimationCameraPosition()
{

    float smoothnessFactor=35*SpiralPoints[700*cameraindex].z +1.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight();
    //timeInterval=10000;
    float timeInterval=smoothnessFactor/1500;
    
//    cout<<"smoothness factor\n"<<smoothnessFactor;
    
    if(startAnimationCounter<=smoothnessFactor-timeInterval)
    {
//        
//        cout<<"\ndifference in the positions "<<tweenvalue;        
//        cout<<"\nCamera Position"<< tweenedCameraPosition.z;
        
       
        tweenvalue = (startAnimationCounter) /smoothnessFactor;
       
        if(tweenvalue<0.98)
        startAnimationCounter+=timeInterval;
        else startAnimationCounter+=(timeInterval);
        
        overshotCameraStartingPosition=tweenedCameraPosition;
    }
    
    //else isstartingAnimationActive=false;
    else {startAnimationCounter=0; startoverShotCameraAnimation=true;}
    
    tweenedCameraPosition.x=ofLerp(0, 0 , tweenvalue);
    tweenedCameraPosition.y=ofLerp(0, 0, tweenvalue);
    tweenedCameraPosition.z=ofLerp(0, 35*SpiralPoints[700*cameraindex].z +21.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight(), tweenvalue);
   
    //cout<<tweenedCameraPosition.z<<"\n";

    
//    cout<<35*SpiralPoints[700*cameraindex].z<<endl;
    
    return tweenedCameraPosition;

}

void testApp::sortImages()
{
   // Create an unordered map ....
    
 //   string path = "/Applications/MAMP/htdocs/25labs/100002627332238/pictures.xml";
    string path="/Users/rahulbudhiraja/Work/of_v0073_osx_release/apps/myApps/Nostalgia_Spiral/bin/data/Images/"+ofToString(userid)+"/pictures.xml";
    
    int imageCounter=1;
    int newScore;
    ImageData imagedataObject;
    
    if(pictures_XML.loadFile(path))
    {
    
        pictures_XML.pushTag("xml");
        pictures_XML.pushTag("ImageList");
        
        for(int i=0;i<pictures_XML.getNumTags("Album");i++)
        {
            pictures_XML.pushTag("Album",i);
//            cout<<pictures_XML.getNumTags("Image")<<endl;;
            
            
            for (int j=0;j<pictures_XML.getNumTags("Image");j++)
            {
                pictures_XML.pushTag("Image",j);
                int score;
                if(pictures_XML.getValue("Tags",0)>15)
                    score=0;
                else  { score=2*pictures_XML.getValue("Likes",0)+3*pictures_XML.getValue("Comments",0);}
                //cout<<score<<endl;
                
                
                imageScores.insert(std::pair<int,int>(score,imageCounter));
                albumScores.insert(std::pair<int,int>(score,i)); // Storing the Album Number ...
                
   /// The New Sorting Algorithm 
                
                imagedataObject.albumnumber= i;
                imagedataObject.imageNumber=imageCounter;
                
                if(pictures_XML.getValue("Tags",0)>0)
                {
                    #ifndef EfficientReorder
                    imagedataObject.isTagged=true;
#endif
                    if(pictures_XML.getValue("Tags",0)>10)
                        imagedataObject.imageScore=0;
                        else 
                    imagedataObject.imageScore=pictures_XML.getValue("Likes",0)+2*pictures_XML.getValue("Comments",0);;
                    taggedImages.push_back(imagedataObject);

                }
                
                
                else
                {
#ifndef EfficientReorder
                    
                   imagedataObject.isTagged=false;
#endif
                   imagedataObject.imageScore=0.5*pictures_XML.getValue("Likes",0)+pictures_XML.getValue("Comments",0);
                   untaggedImages.push_back(imagedataObject);
                }
                
                cout<<"Image Number\t"<<imageCounter<<"\tLikes "<<pictures_XML.getValue("Likes",0)<<"Comments "<<pictures_XML.getValue("Comments",0)<<endl;

                
//                imageDetails.push_back(imagedataObject);
                
                pictures_XML.popTag();
                imageCounter++;
                
            }
            pictures_XML.popTag();
        }
        
    
    }
    
    
/* Debug Statements to check Scores */
    
//    cout<<"Size of Tagged Images"<<taggedimageScores.size()<<endl;
//   cout<<"size of Untagged Images"<<untaggedimageScores.size()<<endl;
//    sort(imageDetails.begin(),imageDetails.end(),imagedataObject);
  
    sort(taggedImages.begin(),taggedImages.end(),imagedataObject);
    sort(untaggedImages.begin(),untaggedImages.end(),imagedataObject);
    
    cout<<"checking .....\n";
    for(int i=0;i<taggedImages.size();i++)
            cout<<taggedImages[i].imageScore<<" \t"<<endl;

     std::multimap<int,int>::iterator it;
     std::multimap<int,int>::iterator albumiterator=albumScores.begin();
    
    for(it=imageScores.begin();it!=imageScores.end();++it)
    {
        cout<<(*it).first<<"  "<<(*it).second<<endl;
        imageData.push_back(ofVec3f((*it).first,(*it).second,(*albumiterator).second));
        albumiterator++;
    
    }
    
    for(int i=0;i<imageData.size();i++)
        cout<<imageData[i]<<endl;
    
//      cout<<imageCounter<<endl;
}

#endif

testApp::testApp(long long int id)
{
    userid=id;
    cout<<"\n User ID:"<<id<<endl;
}

void testApp::pushWigglePositions()
{
    
//    wigglePositions.push_back(ofVec3f(0,0,0));
    
    wigglePositions.push_back(ofVec3f(1,1,0));
    
    wigglePositions.push_back(ofVec3f(1,1,0));
    
    
    wigglePositions.push_back(ofVec3f(0,1,0));
    
    wigglePositions.push_back(ofVec3f(0,1,0));
    
    wigglePositions.push_back(ofVec3f(1,0,0));
    
    wigglePositions.push_back(ofVec3f(1,0,0));
    


}

ofVec3f testApp::wiggle()
{
    //Check if wiggling ...
    
    int maxValue=20; // Change this for adjusting the Maximum Value of the Wiggling ....
    
    
    if(currentwiggleindex%2==0)
    {
        
    if(wiggleAnimationCounter<=maxValue)
            wiggleAnimationCounter+=0.05;
        
    else {
        currentwiggleindex++;
        currentwiggleindex=currentwiggleindex%wigglePositions.size();
        wiggleAnimationCounter=maxValue;
    }
        
        
    }
    
    
    
    else {
        
                   
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

//    cout<<currentwigglePosition<<endl;
    
    // If not wiggling ..change the index ....
    
    
    //If wiggling ,advance the lerp ....
    
    return currentwigglePosition;
}

void testApp::reorder()
{
    std::multimap<int,int>::iterator it;
    std::multimap<int,int>::iterator tempIterator;
    std::multimap<int,int>::iterator thirdtempIterator;
    
    
    //
    std::multimap<int,int>::iterator firstAlbumIterator;
    std::multimap<int,int>::iterator secondAlbumIterator;
    
    
    int imageIterator=0;
    
    cout<<"*************************\n";
    cout<<"Reordering***************\n";
    cout<<endl;
    int albumnumber,tempPicIndex;
    
    firstAlbumIterator=imageScores.begin();
    
    for(it=albumScores.begin();it!=albumScores.end();++it)
    {
        tempIterator=++it;

        it--;

              
        if((tempIterator)==albumScores.end())
            break;

        //tempIterator--;
        
        secondAlbumIterator=++firstAlbumIterator;
        firstAlbumIterator--;
        
        if((*it).second==(*tempIterator).second)
        {
            thirdtempIterator=++tempIterator;
            tempIterator--;
            
            while((*it).second==(*tempIterator).second||(*tempIterator).second==(*thirdtempIterator).second)
            {
                
            tempIterator++;
                secondAlbumIterator++;
//                cout<<"reassigning\n";
                thirdtempIterator++;
            }
            
            cout<<"swapping values "<<(*it).second<<"   "<<(*tempIterator).second;
                
//            tempPicIndex=(*it).second;
//            (*it).second=(*tempIterator).second;
//            (*tempIterator).second=tempPicIndex;
            
//            cout<<"swapping values "<<(*it).second<<"   "<<(*tempIterator).second;

            tempPicIndex=(*firstAlbumIterator).second;
            (*firstAlbumIterator).second=(*secondAlbumIterator).second;
            (*secondAlbumIterator).second=tempPicIndex;
        }
        
        
        cout<<(*it).second<<"::"<<(*tempIterator).second<<endl;
        firstAlbumIterator++;
        //Perform a swap ...
           
    }

}

void testApp::newReorder()
{
    int j,tempVal;
    
//    for(int i=0;i<imageData.size()-3;i++)
//    {
//        j=i+1;
//        
//        if(imageData[i].z==imageData[j].z)
//        {
//            
//            while(imageData[i].z==imageData[j].z||imageData[i].z==imageData[j+1].z)
//            {
////                cout<<"passing"<<endl;
//                j++;// this will help us to find the index so that we can swap the element ..
//
//            }
//            
//            tempVal=imageData[i].z;
//            imageData[i].z=imageData[j].z;
//            imageData[j].z=tempVal;
//            
//
//        }
//        cout<<imageData[i]<<endl;
//    }

    cout<<"reordering";
    
    for(int i=imageData.size()-1;i>1;i--)
    {
        j=i-1;
        
        
        if(imageData[i].z==imageData[j].z)
        {
            cout<<"doing this when i= \t "<<imageData[i].z<<"\t";
            
            while(j>0&&(imageData[i].z==imageData[j].z))
               {
                   j--;
               }
            cout<<" and then j= \t"<<imageData[j].z<<endl;
         
            tempVal=imageData[i-1].z;
            imageData[i-1].z=imageData[j].z;
            imageData[j].z=tempVal;
          
            tempVal=imageData[i-1].y;
            imageData[i-1].y=imageData[j].y;
            imageData[j].y=tempVal;

            
        }
    }
    
 
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

/* Unused */
bool testApp::sortOnImageScore(ImageData l,ImageData r) {
    return l.imageScore<r.imageScore;
}


void testApp::complexReorder()
{
    int ratio= 3; // This ratio defines how many images should be taken from the untagged Images compared to the TaggedImages ....
                  // 2+1 ..
    
    imageData.clear();
    
    
    cout <<"Trying the complex reorder"<<endl;
    int untaggedImageCount=0,taggedImageCount=0,i;
    
    while(untaggedImageCount!=untaggedImages.size())
    {
        
        if(i%ratio==0 && i!=0 && taggedImageCount<taggedImages.size())
        {
            imageDetails.push_back(taggedImages[taggedImageCount]);
            taggedImageCount++;
        }
        else {
            imageDetails.push_back(untaggedImages[untaggedImageCount]);
            untaggedImageCount++;
        }
        i++;
        
    }
    
    // Fill the remaining
    
    while(taggedImageCount<taggedImages.size())
    {
        imageDetails.push_back(taggedImages[taggedImageCount]);
        taggedImageCount++;
    }
    
    #ifndef EfficientReorder
    
    for(int i=0;i<imageDetails.size();i++)
        cout<<imageDetails[i].isTagged<<endl;
#endif
    
    // Sorted Data Structures,How to re-order these so that the same album number isnt the problem ...
    
    int j,tempVal;
     
    for(int i=imageDetails.size()-1;i>=1;i--)
    {
        j=i-1;
        
        
        if(imageDetails[i].albumnumber==imageDetails[j].albumnumber)
        {
            cout<<"doing this when i= \t "<<imageDetails[i].albumnumber<<"\t";
            
            while(j>0&&(imageDetails[i].albumnumber==imageDetails[j].albumnumber))
            {
                j--;
            }
            cout<<" and then j= \t"<<imageDetails[j].albumnumber<<endl;
                      
            std::swap(imageDetails[i-1], imageDetails[j]);
            
        }
    }
    
    cout<<"Now checking the album numbers .";
//
//    for(int i=0;i<imageDetails.size();i++)
//        cout<<imageDetails[i].albumnumber<<endl;
//
}

void testApp::loadImagesandXMLData()
{
    string untaggedDirpath="/Users/rahulbudhiraja/Work/of_v0073_osx_release/apps/myApps/Nostalgia_Spiral/bin/data/Images/"+ofToString(userid)+"/untaggedImages/";
    ofDirectory untaggeddir(untaggedDirpath);
    untaggeddir.allowExt("png");
    untaggeddir.allowExt("jpg");
    untaggeddir.allowExt("gif");
    
    if(untaggeddir.listDir()==0)
    {cout<<"Nothing";   ofExit();}
    
    cout<<untaggeddir.listDir();
    
    cout<<"NUMBER OF FILES"<<untaggeddir.numFiles()<<endl;
    
    ofImage TempImage;
    
       
    //// Load all the Images in the Image Vector ....
    
//    for(int i = 0; i < untaggeddir.numFiles(); i++){
//        if(!TempImage.loadImage(ofToString(untaggedDirpath+ofToString(i)+".jpg")))
//            continue; // WTF Openframeworks ...OSX indexing .. and i+1 is because there is nothing called 0.jpg .....
//        TempImage.resize(TempImage.getWidth(), TempImage.getHeight());
//        
//#ifndef BLUR
//        
//        
//        TempImage.mirror(true,false);
//#endif
//        
//        ImageVector.push_back(TempImage);
//        
//        // cout<<TempImage.getWidth()<<"\t\t"<<TempImage.getHeight()<<"\n\n";
//        TempImage.clear();
//        
//        //        cout<<i<<endl;
//        //        ofLogNotice(dir.getPath(i));
//    }
//    
    int imageCounter=0;
    
    
    string xmlpath="/Users/rahulbudhiraja/Work/of_v0073_osx_release/apps/myApps/Nostalgia_Spiral/bin/data/Images/"+ofToString(userid)+"/imagedata.xml";
    
    ImageData imagedataObject;
    
    // Then Populate the ImageDetails Structure 
    
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
                
//                cout<<j<<endl;
                
                combinedImageObjects.push_back(imagedataObject);
                
                pictures_XML.popTag();
                
                imageCounter++;
                
            }
               pictures_XML.popTag();   
            
           }
    
    }
    
    cout<<combinedImageObjects.size()<<"\t this was the size of untaggedImgeobjects";
    
    ImageVector.clear();
    imageCounter=0;
    
    
    
    
    string taggedDirpath="/Users/rahulbudhiraja/Work/of_v0073_osx_release/apps/myApps/Nostalgia_Spiral/bin/data/Images/"+ofToString(userid)+"/taggedImages/";
    ofDirectory taggeddir(taggedDirpath);
    
    taggeddir.allowExt("png");
    taggeddir.allowExt("jpg");
    taggeddir.allowExt("gif");
    

    
//    for(int i = 0; i < taggeddir.numFiles(); i++){
//        if(!TempImage.loadImage(ofToString(taggedDirpath+ofToString(i)+".jpg")))
//            continue; // WTF Openframeworks ...OSX indexing .. and i+1 is because there is nothing called 0.jpg .....
//        TempImage.resize(TempImage.getWidth(), TempImage.getHeight());
//        
//#ifndef BLUR
//        
//        
//        TempImage.mirror(true,false);
//#endif
//        
//        ImageVector.push_back(TempImage);
//        
//        // cout<<TempImage.getWidth()<<"\t\t"<<TempImage.getHeight()<<"\n\n";
//        TempImage.clear();
//        
//        //        cout<<i<<endl;
//        //        ofLogNotice(dir.getPath(i));
//    }
    
    
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
    
    int ratio= 3; // This ratio defines how many images should be taken from the untagged Images compared to the TaggedImages ....
    // 2+1 ..
    
    
    cout <<"Trying the complex reorder"<<endl;

    int untaggedImageCount=0,taggedImageCount=0,i;
    
    int num_untaggedImages=combinedImageObjects.size(),num_taggedImages=taggedImageObjects.size();
  
    int taggedImgCount=0;
    
//    for(int i=0;i<taggedImageObjects.size();i++)
//    {
//        if(i%ratio==0&&i!=0&&taggedImgCount<num_taggedImages)
//        { std::swap(untaggedImageObjects[i], untaggedImageObjects[num_untaggedImages-1+taggedImgCount]);
//            taggedImgCount++;
//        }
//    }
    
    cout<<"Combining and shuffling the tagged and untagged images";
    for(int i=0;i<combinedImageObjects.size();i++)
    {
        if(i%ratio==0&&i!=0&&taggedImgCount<num_taggedImages)
            combinedImageObjects.insert(combinedImageObjects.begin()+i,taggedImageObjects[taggedImgCount++]);
        
    }

    
//    while(untaggedImageCount!=untaggedImageObjects.size())
//    {
//        
//        if(i%ratio==0 && i!=0 && taggedImageCount<taggedImageObjects.size())
//        {
//            combinedImageObjects.push_back(taggedImageObjects[taggedImageCount]);
//            taggedImageCount++;
//        }
//        else {
//            combinedImageObjects.push_back(untaggedImageObjects[untaggedImageCount]);
////            untaggedImageObjects.pop_back();
//            
//            untaggedImageCount++;
//        }
//        i++;
//        
//    }
    
    
    

    // Sorted Data Structures,How to re-order these so that the same album number isnt the problem ...
    
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
    


//    ofExit();
    
}

ofVec3f testApp::adjustoverShotCameraPosition()
{
    
            
        float smoothnessFactor=35*SpiralPoints[700*cameraindex].z +1.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight();
    //timeInterval=10000;
    float timeInterval=smoothnessFactor/500;
    
    //    cout<<"smoothness factor\n"<<smoothnessFactor;
    
    if(startAnimationCounter<=smoothnessFactor-timeInterval)
    {
        //
        //        cout<<"\ndifference in the positions "<<tweenvalue;
        cout<<"\nCamera Position"<< tweenedCameraPosition.z;
        
        
        tweenvalue = (startAnimationCounter) /smoothnessFactor;
        
        if(tweenvalue<0.98)
            startAnimationCounter+=timeInterval;
        else startAnimationCounter+=(timeInterval);
        
    }

        else isstartingAnimationActive=false;
    
    tweenedCameraPosition.x=ofLerp(0, 35*SpiralPoints[700*cameraindex].x, tweenvalue);
    tweenedCameraPosition.y=ofLerp(0, 35*SpiralPoints[700*cameraindex].y, tweenvalue);
    tweenedCameraPosition.z=ofLerp(overshotCameraStartingPosition.z, 35*SpiralPoints[700*cameraindex].z +1.6*combinedImageObjects[numberofImages-cameraindex].theloadedimage.getHeight(), tweenvalue);
    
    return tweenedCameraPosition;

}

