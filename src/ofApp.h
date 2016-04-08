#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxSimpleGuiToo.h"
//#include "OffAxisProjectionCamera.h"
//#include "AnaglyphCamera.h"
#include "ofCamera.h"
#include "ofVbo.h"

#ifndef DEBUG
#include "ofxKinect.h"
#define USE_KINECT
#endif

#include "ofxOsc.h"
#define PORT 3000 // listen on port 3000
#define NUM_MSG_STRINGS 20

#define CAM_WIDTH 640
#define CAM_HEIGHT 480
/*
class DebugDraw : public ofBaseDraws{
    
public:
    void draw(float x,float y){
        draw(x, y, getWidth(), getHeight());
    }
    void draw(float x,float y,float w, float h){
        for (int i=0; i<layers.size(); i++) {
            layers[i]->draw(x, y, w, h);
        }
    }
    float getWidth(){
        return layers.size() > 0 ? layers[0]->getWidth() : 0;
    }
    float getHeight(){
        return layers.size() > 0 ? layers[0]->getHeight() : 0;
    }
    void addLayer(ofBaseDraws &layer){
        layers.push_back(&layer);
    }
    
protected:
    vector<ofBaseDraws*> layers;
};


class ROI : public ofBaseDraws, public ofRectangle{
    
public:
    void draw(float _x,float _y){
        draw(_x, _y, getWidth(), getHeight());
    }
    void draw(float _x,float _y,float w, float h){
        
        ofPushMatrix();
        ofTranslate(_x, _y);
        ofScale(w/CAM_WIDTH, h/CAM_HEIGHT);
        
        ofSetColor(255, 0, 0);
        ofNoFill();
        ofSetLineWidth(1);
        ofRect(*this);
        
        ofPopMatrix();
    }
    float getWidth(){
        return CAM_WIDTH;
    }
    float getHeight(){
        return CAM_HEIGHT;
    }
};
*/

class ofApp : public ofBaseApp {
    
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void mouseScrolled(int x, int y, float scrollX, float scrollY );
    
    //OSC
    ofxOscReceiver receiver;
    void checkOSC();
    long oscPushTime;
    
    int filter; // which filter -  0.none 1.coordinates 2.vision 3.soundXray 4.composition
    bool filterCoordinates;
    
    bool filterVisible;
    
    bool filterXraySound;
    bool filterComposition;

    
#ifdef USE_KINECT
    ofxKinect kinect;
#else
    ofVideoPlayer kinectPlayer;
    ofImage videoTempImg;
#endif
    ofCamera camera;
    //OffAxisProjectionCamera camera;
    //AnaglyphCamera anaglyphCamera;
    float delta;
    
    ofVbo vbo;
    ofShader shader;
    ofShader shaderGlow;
    vector <ofVec3f> points;
    vector <float> sizes;
    vector <float> starSizes;
    ofTexture texture;
    
    ofFbo leftEyeFBO;
    ofFbo rightEyeFBO;
    
    ofSoundPlayer welcome;
    ofSoundPlayer woosh;
    float soundReqSpeedsAmps;
    
    float blue[4];
    float red[4];
    float cameraEyesOffset;
    
    float offSetX;
    float offSetY;
    
    float cameraZ;
    float speed;
    
    float currPosMin;
    float currPosMax;
    float currSpeed;
    float currSpeedMax;
    
    float starSize;
    float galaxySize;
    int numStars;
    
    float eyeSep;
    
protected:
    
    void initDefaults();
//    void initGui();
    void initFBOs();
    void fillBlobPoly();
    void updateAvgDepth();
    
    int bUserFound;
    bool bGoingForward;
    
    bool bAnaglyph;
    int threshold;
    float avgDepth;
    float avgDepthSmoothed;
    //ROI roi;
    
    float blobSizeMin;
    int kinectAngle, prevKinectAngle;
    
    //DebugDraw depthDebugDraw;
    //DebugDraw thresholdDebugDraw;
    
    ofxCvContourFinder      contourFinder;
    
    ofxCvGrayscaleImage 	depthImage;
    ofxCvGrayscaleImage 	thresholdImage;
    
    ofImage milkyWay;
    ofVbo milkyVbo;
    ofTexture milkyTex;
    ofIcoSpherePrimitive icoSphere;
    
    ofImage spectrumImage;
    ofImage star2Image;
    ofImage starGlowImage;
    double drand();
    double random_normal();
    
    void drawName( const ofVec2f &mousePos, float power, float alpha );
    
};
