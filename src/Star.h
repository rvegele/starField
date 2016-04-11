#pragma once
#include "ofMain.h"
#include "ofVbo.h"

class Star {
    
public:
    Star( int ID, string name );//), int type, int subType, string greekLetter, string name, string regDate );
    //Star( ofVec3f pos, float rad, int type, int subType, string greekLetter, string name, string regDate );
    
    int mID;
    string mName;
    
    float		mDistToMouse;
    
    void update( const ofCamera &cam, float scale );
    void drawName( const ofVec2f &mousePos);
    void setPos ( );
    
    ofVec3f mInitPos;
    ofVec3f mPos;
    ofVbo mSphere;
    float mColor;
    
    ofVec2f     mScreenPos;
    float		mScreenRadius;
    float		mDistToCam;
    float		mDistToCamPer;
    string		mGreek;
    ofTexture   mNameTex;
    float		mRadius;
    float		mRadiusMulti;
    float		mMaxRadius;
    float		mApparentMag;
    float		mAbsoluteMag;
    bool		mIsSelected;
    
    //void update( const ci::Camera &cam, float scale );
    

};