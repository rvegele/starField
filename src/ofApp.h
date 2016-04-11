#pragma once

#include "ofMain.h"
#include "ofCamera.h"
#include "ofVbo.h"
#include "ofVboMesh.h"

#include "ofxOsc.h"
#include "ofxGui.h"

#include "Star.h"
#include <vector>

#define PORT 3000 // listen on port 3000
#define NUM_MSG_STRINGS 20

#define DEBUG

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
    
    // DATA
    void dataImport();
    void createStar( const string &text, int lineNumber );
    void parseData( const string &path );
    vector<Star>	mStars;
    
    //OSC
    ofxOscReceiver receiver;
    void checkOSC();
    long oscPushTime;
    
    //GUI
    //ofxFloatSlider starSize;
    //ofxFloatSlider speed;
    //ofxIntSlider numStars;
    //ofxFloatSlider galaxySize;
    
    ofxPanel gui;
    
    ofParameter<float> starSize;
    ofParameter<float> speed;
    ofParameter<float> galaxySize;
    ofParameter<int> numStars;
    
    ofParameter<int> appearClose;
    ofParameter<int> appearFar;
    
    //ofxColorSlider color;
    //ofxVec2Slider center;
    //ofxIntSlider circleResolution;
    //ofxToggle filled;
    //ofxButton twoCircles;
    //ofxButton ringButton;
    //ofxLabel screenSize;
    
    // CAMERA
    ofCamera camera;
    float delta;
    
    ofVbo vbo;
    ofShader shader;
    ofShader shaderGlow;
    vector <ofVec3f> points;
    vector <float> starSizes;
    ofTexture texture;
    
    ofFbo leftEyeFBO;
    ofFbo rightEyeFBO;
    
    float blue[4];
    float red[4];
    float cameraEyesOffset;
    
    float offSetX;
    float offSetY;
    
    float cameraZ;
    //float speed;
    
    float currPosMin;
    float currPosMax;
    float currSpeed;
    float currSpeedMax;
    
    //float starSize;
    //float galaxySize;
    //int numStars;
    
    float eyeSep;
    
protected:
    
    void initDefaults();
    void initGui();
    void initFBOs();
    bool bGoingForward;
    
    ofImage milkyWay;
    ofVbo milkyVbo;
    ofTexture milkyTex;
    ofIcoSpherePrimitive icoSphere;
    
    ofImage spectrumImage;
    ofImage star2Image;
    ofImage starGlowImage;
    double drand();
    double random_normal();
    
    void drawNames(const ofVec2f &pos, const ofVec2f &mousePos);
    //void drawNames( const ofVec2f &mousePos, float power, float alpha );
    
    ofVboMesh namesMesh;
    int nameIterator;
    vector<string> namePlaceholder;
    
};
