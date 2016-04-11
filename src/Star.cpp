#include "star.h"

Star::Star( int ID, string name, string greekLetter )/* : mPos( pos ), mApparentMag( appMag ), mAbsoluteMag( absMag ), mColor( color ), mName( name )*/{
    
    mID = ID;

    mGreekLetter = greekLetter;
    
    mName = mGreekLetter;
    //mName = mGreekLetter + " " + mName;
    
    mIsSelected = false;
    
    //mInitPos		= mPos;
    mDistToMouse	= 1000.0f;
    //mIsSelected		= false;
    //mRadius			= ( 10.0f - mAbsoluteMag ) * 0.025f;
    //mRadiusMulti	= 1.0f; // not implemented yet
    
    /*
    if( mName.length() > 1 && appMag < 6.0f ){
     TextLayout layout;
     layout.clear( ColorA( 0.0f, 0.0f, 0.0f, 0.0f ) );
     layout.setFont( fontM );
     layout.setColor( Color( 1.0f, 1.0f, 1.0f ) );
     layout.addLine( name );
     layout.setFont( fontS );
     layout.setLeadingOffset( 3 );
     layout.addLine( spectrum );
     mNameTex = gl::Texture( layout.render( true, false ) );
     mSphere.setCenter( mPos );
     mSphere.setRadius( mRadius );
     }
     */
}

//Star::Star( ofVec3f pos, float rad, int type, int subType, string greekLetter, string name, string regDate )/* : mPos( pos ), mApparentMag( appMag ), mAbsoluteMag( absMag ), mColor( color ), mName( name )*/{
    
//    mInitPos = pos;
//    mRadius = rad;
//    mName = name;
//    mGreek = greekLetter;
    
    //mInitPos		= mPos;
    //mDistToMouse	= 1000.0f;
    //mIsSelected		= false;
    //mRadius			= ( 10.0f - mAbsoluteMag ) * 0.025f;
    //mRadiusMulti	= 1.0f; // not implemented yet
    
    /*
     if( mName.length() > 1 && appMag < 6.0f ){
     TextLayout layout;
     layout.clear( ColorA( 0.0f, 0.0f, 0.0f, 0.0f ) );
     layout.setFont( fontM );
     layout.setColor( Color( 1.0f, 1.0f, 1.0f ) );
     layout.addLine( name );
     layout.setFont( fontS );
     layout.setLeadingOffset( 3 );
     layout.addLine( spectrum );
     mNameTex = gl::Texture( layout.render( true, false ) );
     mSphere.setCenter( mPos );
     mSphere.setRadius( mRadius );
     }
     */
//}

void Star::update( const ofCamera &cam, float scale ) {
    //mPos		= mInitPos * scale;
    //mSphere.setCenter( mPos );
    
    //mScreenPos		= cam.worldToScreen( mPos, app::getWindowWidth(), app::getWindowHeight() );
    //mDistToCam		= -cam.worldToEyeDepth( mPos );
    //mDistToCamPer	= math<float>::min( mDistToCam * 0.01f, 1.0f );
    //mScreenRadius	= cam.getScreenRadius( mSphere, app::getWindowWidth(), app::getWindowHeight() );
}

void Star::drawName( const ofVec2f &pos )
{
    ofDrawBitmapStringHighlight(mName, ofPoint(pos.x,pos.y));
 /*
     if( mDistToCam > 0.0f && mNameTex ){
     float per = constrain( 1.0f - mDistToCam * 0.0000375f, 0.0f, 1.0f );
     per *= per * per;
     
     ofVec2f dirToMouse	= mScreenPos - mousePos;
     mDistToMouse		= dirToMouse.length();
     
     if( mDistToMouse < 40.0f )
     per += 1.0f - mDistToMouse/40.0f;
     
     if( mIsSelected )
     per = 1.0f;
     
     if( per > 0.05f ){
         
     gl::color( ColorA( power, power, power, per * alpha ) );
     mNameTex.enableAndBind();
     gl::draw( mNameTex, mScreenPos + Vec2f( mScreenRadius + 35.0f, -28.0f ) );
     mNameTex.disable();
     
     gl::color( ColorA( power, power, power, per * 0.4f * alpha ) );
     Vec2f p1 = mScreenPos;
     Vec2f p2 = p1 + Vec2f( mScreenRadius + 35.0f, -13.0f );
     Vec2f p3 = mScreenPos + Vec2f( mScreenRadius + 35.0f + mNameTex.getWidth(), -13.0f );
     gl::drawLine( p1, p2 );
     gl::drawLine( p2, p3 );
     }
     }
  */
}
  
