#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{

    ofBackground(0, 0, 0);
    
    generateCircularSpiral();
    loadImagesFromDirectory();
    
#ifdef DEBUGMODE
    camera.setDistance(200);
    camera.setFarClip(1000000);

#else 
    
    sortImages();
    camera.setFov(40);
    camera.setFarClip(10000000);
    cameraindex=ImageVector.size()-1;
    camera.setPosition(cameraStartPosition);
    animationMode=false;
    isstartingAnimationActive=true;
    animationStartingPosition=ofVec3f(0,0,0);
    startAnimationCounter=0;
    zdistanceFactor=35;
    pushWigglePositions();
    currentwiggleindex=0;
    wiggleAnimationCounter=0;
//    reorder();
    newReorder();
#endif
//    ofExit();
 
    NostalgiaFont.loadFont("asyouwish.ttf", 300);
//    NostalgiaFont.setLetterSpacing(30);
    NostalgiaFont.setLineHeight(200);
    ofEnableAlphaBlending();
  
   
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw()
{

	ofBackground(0, 0, 0);
    
    ofSetColor(255,255,255);
    
#ifndef DEBUGMODE

    if(isstartingAnimationActive)
    {
        camera.setPosition(startAnimationCameraPosition());
    }
    
    else
    {

    if(animationMode)
        camera.setPosition(animate(cameraindex+1, cameraindex)+ofVec3f(0,0,800));
    else
//    camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,1000)+wigglePositions[rand()%wigglePositions.size()]);
    {
//        if(currentwiggleindex%2==0)
        camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,800)+wiggle());
//        else camera.setPosition(35*SpiralPoints[700*cameraindex]+ofVec3f(0,0,1000)-wiggle()); // SO that the camera goes backward ..
        
    }
        
    }
//    cout<<cameraindex<<endl<<cameraEndPosition;
    
    
    
    
#endif
    camera.begin();

    
    
//    for(int i=0;i<SpiralPoints.size();i++)
//    {
//     
//        // ofSphere(SpiralPoints[i].x,SpiralPoints[i].y,SpiralPoints[i].z,0.4);
////        if(i%330!=0)
////            continue;
//
//       // ofRect(SpiralPoints[i].x-2,SpiralPoints[i].y-1,SpiralPoints[i].z,4,2);
//
//    }
    
//    ofLine(-100,0,100,0);
    
    ofPushMatrix();
    ofRotateX(180);
//    ofScale(400,400,400);
//    NostalgiaFont.drawString("NOSTALGIA", 0, 0);
    ofPopMatrix();
    drawImages();
    
    camera.end();
  

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key=='f'||key=='F')
    {
        ofToggleFullscreen();
    }
    
#ifndef DEBUGMODE    
    
    else if(key==OF_KEY_UP&&cameraindex!=ImageVector.size()-1&&!animationMode)
    {
        cameraindex++;
        animationMode=true;
    
    }
    else if(key==OF_KEY_DOWN&&cameraindex!=0&&!animationMode)
    {
        
    cameraindex--;
    animationMode=true;
      animationCounter=0;
    
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


void testApp::generateSpiral()
{
    float h=1000,r=100,a=100;
    float x,y,z;
    
    for(float R=1000;R>=1;R-=0.5)
    {
       x=((h-z)/h)*R*cos(a*z);
       y=((h-z)/h)*R*sin(a*z);
       z=R;
       SpiralPoints.push_back(ofVec3f(x,y,z));
        
    }
    
}

void testApp::generateCircularSpiral()
{
    int spreadDistance=30;
    
    
    // Conical Concentric Circles .....
    
//    for(int r=0;r<200;r+=spreadDistance)
    
//    {
//        for(float angle=0;angle<=360;angle+=4)
//            SpiralPoints.push_back(ofVec3f(r*cos(ofDegToRad(angle)),r*sin(ofDegToRad(angle)),r));
//    }    

    
    
/// Conical Helix ..
    
    float height=10000,radius=100,ang_freq=3;
    

            for(float angle=0;angle<=3600;angle+=0.01)
            {

                //radius=angle;
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
    
   // string path = "/Applications/MAMP/htdocs/25labs/100002627332238/";

    string path="/Applications/MAMP/htdocs/25labs/"+ofToString(userid)+"/";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.allowExt("gif");
    
    cout<<dir.listDir();
    
    cout<<dir.numFiles();
    
    ofImage TempImage;
    
    for(int i = 0; i < dir.numFiles(); i++){
        TempImage.loadImage(ofToString(path+ofToString(i+1)+".jpg")); // WTF Openframeworks ...OSX indexing .. and i+1 is because there is nothing called 0.jpg .....
        TempImage.resize(TempImage.getWidth(), TempImage.getHeight());
        TempImage.mirror(true,false);
        ImageVector.push_back(TempImage);
        // cout<<TempImage.getWidth()<<"\t\t"<<TempImage.getHeight()<<"\n\n";
        TempImage.clear();
//        ofLogNotice(dir.getPath(i));
    }
   
}



void testApp::drawImages()
{
    
    std::multimap<int,int>::iterator it;
    int imageIterator=0;
    
    
//    cout<<"Starting";
    
//    for(it=imageScores.begin();it!=imageScores.end();++it)

    for(int i=0;i<imageData.size();i++)
    {
        
        ofPushMatrix();
//        int index=(*it).second-1;//ofRandom(30,70);
        int index=imageData[i].y-1;
//        SpiralPoints[i].z*=600;
//       cout<<"Image Number"<<imageIterator<<"\t"<<index<<endl;
        
//        ofTranslate(35*SpiralPoints[700*imageIterator].x,35*SpiralPoints[700*imageIterator].y,35*SpiralPoints[700*imageIterator].z);
    ofTranslate(35*SpiralPoints[700*i].x,35*SpiralPoints[700*i].y,35*SpiralPoints[700*i].z);
        
        ; //cout<<35*SpiralPoints[700*i]<<"\t";
        
        
        
        ImageVector[index].draw(-ImageVector[index].getWidth()/2,-ImageVector[index].getHeight()/2);
        ofPopMatrix();
        
        
        imageIterator++;
        //if(i>=1)
            ;// ofLine(35*SpiralPoints[700*i],35*SpiralPoints[700*(i-1)]);
    }
}

#ifndef DEBUGMODE

ofVec3f testApp::animate(int pos1, int pos2)
{
    
    float smoothnessFactor=2400,timeInterval=5;
    
        if(animationCounter<=smoothnessFactor-timeInterval)
            { tweenvalue = (animationCounter) /smoothnessFactor;
                    animationCounter+=timeInterval;
            }
    else {
        tweenvalue=0;
    animationMode=false;
        return ofVec3f(35*SpiralPoints[700*pos2].x,35*SpiralPoints[700*pos2].y,35*SpiralPoints[700*pos2].z);
    }
    tweenedCameraPosition.x=ofLerp(35*SpiralPoints[700*pos1].x,35*SpiralPoints[700*pos2].x,tweenvalue);
    tweenedCameraPosition.y=ofLerp(35*SpiralPoints[700*pos1].y,35*SpiralPoints[700*pos2].y,tweenvalue);
    tweenedCameraPosition.z=ofLerp(35*SpiralPoints[700*pos1].z,35*SpiralPoints[700*pos2].z,tweenvalue);
//     cout<<tweenvalue<<"\n";
    
    return tweenedCameraPosition;
    
}

ofVec3f testApp::startAnimationCameraPosition()
{

    float smoothnessFactor=35*SpiralPoints[700*cameraindex].z,timeInterval=29;
    
    if(startAnimationCounter<=smoothnessFactor-timeInterval)
    { tweenvalue = (startAnimationCounter) /smoothnessFactor;
        startAnimationCounter+=timeInterval;
    }
    
    else isstartingAnimationActive=false;
    
    tweenedCameraPosition.x=ofLerp(0, 0, tweenvalue);
    tweenedCameraPosition.y=ofLerp(0, 0, tweenvalue);
    tweenedCameraPosition.z=ofLerp(0, 35*SpiralPoints[700*cameraindex].z +1000, tweenvalue);
    //cout<<tweenedCameraPosition.z<<"\n";

    
//    cout<<35*SpiralPoints[700*cameraindex].z<<endl;
    
    return tweenedCameraPosition;

}

void testApp::sortImages()
{
   // Create an unordered map ....
    
 //   string path = "/Applications/MAMP/htdocs/25labs/100002627332238/pictures.xml";
    string path="/Applications/MAMP/htdocs/25labs/"+ofToString(userid)+"/pictures.xml";
    
    int imageCounter=1;
    
   
    
    if(pictures_XML.loadFile(path))
    {
    
        pictures_XML.pushTag("xml");
        pictures_XML.pushTag("ImageList");
        
        for(int i=0;i<pictures_XML.getNumTags("Album");i++)
        {
            pictures_XML.pushTag("Album",i);
            cout<<pictures_XML.getNumTags("Image")<<endl;;
            
            
            for (int j=0;j<pictures_XML.getNumTags("Image");j++)
            {
                pictures_XML.pushTag("Image",j);
                cout<<pictures_XML.getValue("Likes",0)<<endl;
                int score;
                if(pictures_XML.getValue("Tags",0)>15)
                    score=0;
                else  { score=pictures_XML.getValue("Likes",0)+2*pictures_XML.getValue("Tags",0)+2*pictures_XML.getValue("Comments",0);}
                //cout<<score<<endl;
                imageScores.insert(std::pair<int,int>(score,imageCounter));
                albumScores.insert(std::pair<int,int>(score,i)); // Storing the Album Number ...		
                pictures_XML.popTag();
                imageCounter++;
                
            }
            pictures_XML.popTag();
        }
        
    
    }
    
  
    
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
    
    wigglePositions.push_back(ofVec3f(0,0,1));
    
    wigglePositions.push_back(ofVec3f(0,0,1));
    
    
    wigglePositions.push_back(ofVec3f(0,1,0));
    
    wigglePositions.push_back(ofVec3f(0,1,0));
    
    wigglePositions.push_back(ofVec3f(1,0,0));
    
    wigglePositions.push_back(ofVec3f(1,0,0));
    
    
//  new experiments ...
    
//    wigglePositions.push_back(ofVec3f(1,1,1));
//    
//    wigglePositions.push_back(ofVec3f(1,1,1));
//    
//    
//    wigglePositions.push_back(ofVec3f(0,1,1));
//    
//    wigglePositions.push_back(ofVec3f(0,1,1));
//    
//    
//    wigglePositions.push_back(ofVec3f(1,0,1));
//    
//    wigglePositions.push_back(ofVec3f(1,0,1));
//    
//    
//    wigglePositions.push_back(ofVec3f(1,1,0));
//    
//    wigglePositions.push_back(ofVec3f(1,1,0));

}

ofVec3f testApp::wiggle()
{
    //Check if wiggling ...
    
    int maxValue=20; // Change this for adjusting the Maximum Value of the Wiggling ....
    
    
    if(currentwiggleindex%2==0)
    {
        
    if(wiggleAnimationCounter<=maxValue)
            wiggleAnimationCounter+=0.01;
        
    else {
        currentwiggleindex++;
        currentwiggleindex=currentwiggleindex%wigglePositions.size();
        wiggleAnimationCounter=maxValue;
    }
        
        
    }
    
    else {
        
                   
            if(wiggleAnimationCounter>=0)
                wiggleAnimationCounter-=0.01 ;
            
            else {
                currentwiggleindex++;
                currentwiggleindex=currentwiggleindex%wigglePositions.size();
                wiggleAnimationCounter=0;
            }
            
            
        
        
    }

    
    currentwigglePosition.x=wigglePositions[currentwiggleindex].x*wiggleAnimationCounter;
    currentwigglePosition.y=wigglePositions[currentwiggleindex].y*wiggleAnimationCounter;
    currentwigglePosition.z=wigglePositions[currentwiggleindex].z*wiggleAnimationCounter;

    cout<<currentwigglePosition<<endl;
    
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
    
    for(int k=0;k<imageData.size();k++)
        cout<<imageData[k]<<endl;
    
    
}