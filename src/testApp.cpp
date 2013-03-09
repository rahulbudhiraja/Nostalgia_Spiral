#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{

    ofBackground(0, 0, 0);
    
    generateCircularSpiral();
    camera.setDistance(200);
    camera.setFarClip(1000000);
   // camera.rotate(180, 0, 1, 0);
    
    loadImagesFromDirectory();
    AssignRandomPositions();
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
    
    camera.begin();
    
    for(int i=0;i<SpiralPoints.size();i++)
    {
       //ofSetColor(ofRandom(255),ofRandom(255),ofRandom(255));
        
//        ofSphere(SpiralPoints[i].x,SpiralPoints[i].y,SpiralPoints[i].z,0.4);
        ofRect(SpiralPoints[i].x-2,SpiralPoints[i].y-1,SpiralPoints[i].z,4,2);
        
    }
    
//    ofLine(-100,0,100,0);
    
    drawImages();
    camera.end();
        
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key=='f'||key=='F')
    {
        ofToggleFullscreen();
    }
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
//        
//
//        
//    {
//        for(float angle=0;angle<=360;angle+=4)
//            SpiralPoints.push_back(ofVec3f(r*cos(ofDegToRad(angle)),r*sin(ofDegToRad(angle)),r));
//    }
//    
//    }
    
    
/// Conical Helix ..
    

            for(float angle=0;angle<=3600;angle+=1)
                SpiralPoints.push_back(ofVec3f(angle*cos(ofDegToRad(6*angle)),angle*sin(ofDegToRad(6*angle)),angle));
        
        



}


void testApp::loadImagesFromDirectory()
{
    
    string path = "/Applications/MAMP/htdocs/25labs/100002627332238";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.allowExt("gif");
    dir.listDir();
    
    cout<<dir.numFiles();
    ofImage TempImage;
    
    for(int i = 0; i < dir.numFiles(); i++){
        TempImage.loadImage(dir.getPath(i));
        TempImage.resize(TempImage.getWidth()/2, TempImage.getHeight()/2);
        TempImage.mirror(false,true);
        ImageVector.push_back(TempImage);
        // cout<<TempImage.getWidth()<<"\t\t"<<TempImage.getHeight()<<"\n\n";
        TempImage.clear();
    }
    
}

void testApp::AssignRandomPositions()
{
    for(int i=0;i<ImageVector.size();i++)
        ImagePositions.push_back(ofVec3f(ofRandom(0,1.5*ofGetWidth()),ofRandom(0,1.5*ofGetHeight()),ofRandom(100000,-10000)));
    
}

void testApp::drawImages()
{
    for(int i=0;i<ImageVector.size();i++)
    {
        ofPushMatrix();
        ofTranslate(ImagePositions[i].x,ImagePositions[i].y,ImagePositions[i].z);
        ;// cout<<ImagePositions[i].z<<"\t";
        ImageVector[i].draw(0,0);
        ofPopMatrix();
    }
}

