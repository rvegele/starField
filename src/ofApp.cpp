#include "ofApp.h"
#include <random>

#include <sys/stat.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <errno.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <sys/mman.h>
//#include <sys/types.h>
//#include <time.h>

// random stuff
std::random_device rd;
std::mt19937 e2(rd());
std::normal_distribution<float> dist(0.40, 0.15);

void ofApp::setup() {
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofDisableArbTex(); // GL_REPEAT for texture wrap only works with NON-ARB textures //
    
    ofLoadImage(texture,"/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/star.png");
    shader.load("/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/shader.vert", "/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/shader.frag");
    shaderGlow.load("/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/shaderGlow.vert", "/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/shaderGlow.frag");
    
    //ofLoadImage(milkyTex, "/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/milkyWay2.jpg");
     milkyWay.load("/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/milkyWay2.jpg");
    
     spectrumImage.load("/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/spectrum2.png");
     star2Image.load("/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/star.png");
     starGlowImage.load("/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/starGlow.png");
    
    
    //milkyWay.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );
    //milkyWay.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );
    //icoSphere.setRadius( 195000.0f );
    
    icoSphere.setRadius( 5000 );
    icoSphere.setPosition(0.0, 0.0, 0.0);
    icoSphere.setResolution(4);
    //icoSphere.rotate(50, ofVec3f(0.0, 1.0, 0.0));
    icoSphere.rotate(110, ofVec3f(0.0, 0.0, 1.0));
    icoSphere.rotate(-40, ofVec3f(1.0, 0.0, 0.0));

    
    dataImport();
    
    initDefaults();
    initGui();
    
    
#ifndef DEBUG
    ofHideCursor();
    ofSetFullscreen(true);
#endif
    
    initFBOs();
    
    camera.setFarClip(100000);
    
    // OSC
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
//    filter = 0;
}

void ofApp::initGui() {
    
    gui.setup();
    //gui.add(starSize.set("starSize", starSize, 1.0f, 30000.0f));
    gui.add(speed.set("speed", speed, 1.0f, 30000.f));
    gui.add(galaxySize.set("galaxySize", galaxySize, 320, 15000.f));
    gui.add(numStars.set("numStars", numStars, 250, 2000.f));

    gui.add(appearClose.set("appearClose", appearClose, -20000, 50000));
    gui.add(appearFar.set("appearFar", appearFar, 0, 100000));
    
}
void ofApp::dataImport() {
    // DATA FILE
    int i;
    int fd;
    
    fd = open("/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/random_names.csv", O_RDONLY);
    //fd = open(file, O_RDONLY);
    char *map;  /* mmapped array of int's */
    
    struct stat buffer;
    int status = fstat(fd, &buffer);
    
    cout << "number of data items in file: " << buffer.st_size/sizeof(char) << "\n";
    if (fd == -1) {
        perror("Error opening file for reading");
        exit();
        //exit(EXIT_FAILURE);
    }
    
    map = (char*) mmap(NULL, buffer.st_size/sizeof(char), PROT_READ, MAP_SHARED, fd, 0);
    i = 0;
    int count = 0;
    while(map[i]) {
        if(map[i] == '\n') {
            count++;
        }
        i++;
    }
    cout << "lines: " << count << "\n";
    
    parseData("/Users/regi/Documents/of_v0.9.3_osx_release/apps/myApps/starField/data/random_names.csv");
    
    cout << "number of names: " << mStars.size() << std::endl;
    //cout << mStars.at(0).mName << "\n";
}

void ofApp::initFBOs()
{
    ofFbo::Settings s;
    s.width = ofGetWindowWidth();
    s.height = ofGetWindowHeight();
    //s.width				= ofGetWidth();
    //s.height			= ofGetHeight();
    s.internalformat    = GL_RGBA;
    leftEyeFBO.allocate(s);
    rightEyeFBO.allocate(s);
    
    leftEyeFBO.begin();
    ofClear(0, 0, 0, 0);
    leftEyeFBO.end();
    
    rightEyeFBO.begin();
    ofClear(0, 0, 0, 0);
    rightEyeFBO.end();
}

void ofApp::initDefaults()
{
    cameraZ = 0;
    bGoingForward = false;
    
    blue[0] = 0;
    blue[1] = 255;
    blue[2] = 255;
    blue[3] = 255;
    
    red[0] = 255;
    red[1] = 0;
    red[2] = 0;
    red[3] = 255;
    
    cameraEyesOffset = 4.0;
    offSetX = 0.53;
    offSetY = 0.5;
    
    starSize = 64.0;
    
    speed = 4000;
    numStars = 1000;
    galaxySize = 10000;
    appearClose = -10000;
    appearFar = 70000;
    
    currPosMin = currPosMin = 0;
    currSpeed = currSpeedMax = 0;
    
    eyeSep = 1;
    
    namesMesh.getVertices().resize(numStars);
    namesMesh.getColors().resize(numStars);
    namesMesh.getNormals().resize(numStars,ofVec3f(0));
    
    // from 0 to <1000>
    for (int i=points.size(); i < numStars; i++) {
        
        //global star iterator pradedam nuo 0 ir data imam is failo,
        //o tada kai maximumas resetinam iki 0;
        
        // star initial position //
        ofPoint p = ofPoint(ofRandom(-galaxySize, galaxySize), ofRandom(-galaxySize, galaxySize), /*ofRandom(-70000,10000));*/
                    cameraZ - ofRandom(appearClose, appearFar));
                    //cameraZ - 100000);
        points.push_back(p);
        
        namesMesh.getVertices()[i].set(p.x, p.y, p.z);
        //sizes.push_back(cameraZ-p.z);
        
        //cout << dist(e2) << "\n";
        
        // manual version
        //float sSize = (float)random_normal();
        //sSize = ofClamp(sSize, -1.0, 1.0);
        //sSize = ofMap(sSize, -1.0, 1.0, 0.0, 1.0);
        
        // auto version
        float sSize = dist(e2);
        sSize = ofClamp(sSize, 0.0, 1.0); //cout << sSize << "\n";
        starSizes.push_back(sSize);
    }
    
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY ) {
    //bool isFrameNew = true;
    delta = scrollY;//*0.05;
}

void ofApp::update() {
    
    // OSC
    checkOSC();

    delta *= 0.05; //cout << delta << "\n";
    currSpeed += ( delta - currSpeed ) * 0.15;
    
    // rotate background
    icoSphere.rotate(delta, ofVec3f(0.0, 1.0, 0.0));
    
    if(currSpeed > currSpeedMax) currSpeedMax = currSpeed;
    cameraZ += -currSpeed * (currSpeed>0?2:1) * speed;
    
    
    /*
    for (int i=0; i<points.size(); i++) {
        points[i].z += -currSpeed * (currSpeed>0?2:1) * speed;
        //cout << points[i].z << "\n";
    }
    */
    
    /*
    if(!bGoingForward && delta > 0.005) {
        bGoingForward = true;
        //woosh.play();
    } else if (bGoingForward && delta < -0.001) {
        bGoingForward = false;
    }
    */
    
    //cout << cameraZ << "\n";
    //for (int i = points.size()-1; i > 0; i--) {
    // this one works
    for (int i = 0; i < points.size(); i++) {
        /*if( points[i].z > 10000) {*/
        if (points[i].z > cameraZ/* + 10000*/) {       // uncomments if planning to go backwards!
            namesMesh.removeVertex(i);
            //namesMesh.getVertices()[i].set(0.0, 0.0, 0.0);
            //namesMesh.getVertex(i);
            
            points.erase(points.begin()+i);            // erase the point
            //sizes.erase(sizes.begin()+i);              // erase the point size
            starSizes.erase(starSizes.begin()+i);
            
            i--;
        }
    }
    
    ofPoint p;
    /*
    // random stuff
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<float> dist(0.40, 0.15);
    */
    
    for (int i=points.size(); i < numStars; i++) {
        p = ofPoint(ofRandom(-galaxySize, galaxySize), ofRandom(-galaxySize, galaxySize), /*ofRandom(-70000,10000));*/
                    //cameraZ - ofRandom(-10000, 70000));
                    cameraZ - ofRandom(appearClose, appearFar)); // 20000 - 70000
        points.push_back(p);
        namesMesh.addVertex(ofVec3f(p.x, p.y, p.z));
        //namesMesh.getVertices()[i].set(p.x, p.y, p.z);
    
        //namesMesh.getVbo().setAttributeData(shader.getLocation("circlePosition"), posVector.data(), 3, posVector.size());
        //circleMesh.setAttributeDivisor(shader.getLocation("circlePosition"),1); // this tells that the circlePosition attribute is passed once per instance instead of once per vertex
        //sizes.push_back(cameraZ-p.z);
        
        //cout << dist(e2) << "\n";
        
        // manual version
        //float sSize = (float)random_normal();
        //sSize = ofClamp(sSize, -1.0, 1.0);
        //sSize = ofMap(sSize, -1.0, 1.0, 0.0, 1.0);
        
        // auto version
        float sSize = dist(e2);
              sSize = ofClamp(sSize, 0.0, 1.0); //cout << sSize << "\n";
        starSizes.push_back(sSize);
    }
    int total = (int)points.size();
    //cout << total << "\n";
   
    vbo.setVertexData(&points[0], total, GL_DYNAMIC_DRAW);
    namesMesh.setUsage( GL_DYNAMIC_DRAW );
    namesMesh.setMode(OF_PRIMITIVE_POINTS);
    
    //ofVec3f myCamGlobalPosition = camera.getGlobalPosition();
    //ofQuaternion myCamRotation = camera.getGlobalOrientation();
    //cout <<myCamRotation << "\n";
}

void ofApp::draw() {
    
    //ofEnableAlphaBlending();
    ofBackground(0, 0, 0);

    //ofEnableDepthTest();
    ofDisableDepthTest();
    
    //ofEnablePointSprites();
    
    //ofPushMatrix();
        //leftEyeFBO.begin();
        // MILKY WAY
        //glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //icoSphere.rotate(0.005, ofVec3f(0.0, 1.0, 0.0));
    ofClear(0, 0, 0, 0);
    //ofPushMatrix();
        camera.setPosition(0, 0, 0);
        camera.begin();
    //icoSphere.set(10, 2);
    // DRAW MILKYWAY
    //    gl::pushMatrices();
    //    gl::translate( mSpringCam.getCam().getEyePoint() );
    //    gl::rotate( Vec3f( 75.0f, 0.0f, 0.0f ) );
    //    gl::color( ColorA( 1.0f, 1.0f, 1.0f, power * mScalePer ) );
    //ofClear(1.0, 1.0, 1.0, 1.0);
    //    mMilkyWayTex.bind();
    
    milkyWay.getTexture().bind();
    icoSphere.draw(); // milky way sphere
    camera.end();
    
    //ofPopMatrix();
    
    
    //glDisable(GL_TEXTURE_2D);
    // MILKY WAY END
    
    glDisable(GL_CULL_FACE);
    //ofDisableAlphaBlending();
    
    //leftEyeFBO.end();

    //shader.begin();
    //shader.setUniform1f("starSize", starSize);
    
    //texture.bind();
    /*
    //if(bAnaglyph) {
    */
      /*
       
        
        // right
     */
    
    ofEnablePointSprites();
    
    // right
    //leftEyeFBO.begin();
    //ofClear(0, 0, 0, 0);
    //ofSetColor(ofColor(blue[0], blue[1], blue[2], blue[3]));
    //camera.setPosition(-cameraEyesOffset*eyeSep, 0, cameraZ);
    //camera.begin(/*offSetX-cameraEyesOffset*0.01, offSetY*/);
    //vbo.draw(GL_POINTS, 0, (int)points.size());
    //camera.end();
    //leftEyeFBO.end();
    
    /*
    leftEyeFBO.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(ofColor(blue[0], blue[1], blue[2], blue[3]));
    ofEnablePointSprites();
    camera.setPosition(-cameraEyesOffset*eyeSep, 0, cameraZ);
    //camera.setPosition(0, 0 ,cameraZ);
    camera.begin();
    shader.begin();
    texture.bind();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    vbo.draw(GL_POINTS, 0, (int)points.size());
    ofDisableBlendMode();
    texture.unbind();
    shader.end();
    camera.end();
    ofDisablePointSprites();
    ofDisableAlphaBlending();
    leftEyeFBO.end();
    */
    
    
    // left
    //rightEyeFBO.begin();
    //ofClear(0, 0, 0, 0);
    //ofSetColor(ofColor(red[0], red[1], red[2], red[3]));
    //camera.setPosition(cameraEyesOffset*eyeSep, 0, cameraZ);
    //camera.begin(/*offSetX+cameraEyesOffset*0.01, offSetY*/);
    //vbo.draw(GL_POINTS, 0, (int)points.size());
    //camera.end();
    //rightEyeFBO.end();
    //texture.unbind();
    //shader.end();
    //ofDisablePointSprites();
    
    /*
    rightEyeFBO.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(ofColor(red[0], red[1], red[2], red[3]));
    ofEnablePointSprites();
    camera.setPosition(cameraEyesOffset*eyeSep, 0, cameraZ);
    //camera.setPosition(0, 0 ,cameraZ);
    camera.begin();
    shader.begin();
    texture.bind();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    vbo.draw(GL_POINTS, 0, (int)points.size());
    ofDisableBlendMode();
    texture.unbind();
    shader.end();
    camera.end();
    ofDisablePointSprites();
    ofDisableAlphaBlending();
    rightEyeFBO.end();
    */
    
    
    // RESULT
    //ofSetColor(255, 255, 255);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    //leftEyeFBO.draw(0, 0);
    //rightEyeFBO.draw(0, 0);
    //ofDisableBlendMode();
    //}
    //else {
    

    
    
        /* THIS WORKS WITHOUT THE FBO - THIS IS THE REGULAR VERSION */
    
    
    rightEyeFBO.begin();
    ofClear(0, 0, 0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
            // BEGIN STAR GLOW SHADER
            shaderGlow.begin();
            ofSetColor(ofColor(0.0, 0.0, 0.0, 255.0));
    
            camera.setPosition(0, 0, cameraZ);
            camera.begin();
    
            shaderGlow.setUniformTexture("tex0", star2Image.getTexture(), 0);
            shaderGlow.setUniformTexture("tex1", spectrumImage.getTexture(), 1);
            shaderGlow.setUniformTexture("tex2", starGlowImage.getTexture(), 2);
    
            for (int u = 0; u < (int)points.size()-1; u++) {
                shaderGlow.setUniform1f("starSize", starSizes[u]);
                shaderGlow.setUniform1f("time", (float)ofGetElapsedTimef() );
                //glPointSize(starSizes[u]);
                vbo.draw(GL_POINTS, u, 1);
                //vbo.draw(GL_POINTS, 0, (int)points.size());
            }
            camera.end();
            shaderGlow.end();
            texture.unbind();
            // END STAR GLOW SHADER
    
            // BEGIN INSIDE STAR SHADER
            shader.begin();
            ofSetColor(ofColor(0.0, 0.0, 0.0, 255.0));
            camera.setPosition(0, 0, cameraZ);
            camera.begin();
            shader.setUniformTexture("tex0", star2Image.getTexture(), 0);
            shader.setUniformTexture("tex1", spectrumImage.getTexture(), 1);
            shader.setUniformTexture("tex2", starGlowImage.getTexture(), 2);
    
            for (int u = 0; u < (int)points.size()-1; u++) {
                
                shader.setUniform1f("starSize", starSizes[u]);
                shader.setUniform1f("time", (float)ofGetElapsedTimef() );
                
                //glPointSize(starSizes[u]);
                vbo.draw(GL_POINTS, u, 1);
                //vbo.draw(GL_POINTS, 0, (int)points.size());
                
                //drawNames(ofVec2f(points[u].x, points[u].y), ofVec2f(ofGetMouseX(), ofGetMouseY()));
            }
            texture.unbind();
            camera.end();
            shader.end();
    
    
            // END INSIDE STAR SHADER
    
    
            // cycle through all the stars
            // assign star names to stars in range z value
            // if run out of star names, go to the beginning of the star name list
    
            // DRAW NAMES
            camera.begin();
            camera.setPosition(0, 0, cameraZ);
            //ofSetColor(ofColor::red);
            //glPointSize(15);
            //namesMesh.drawVertices();
            //namesMesh.draw();
            camera.end();
            // DRAW NAMES END
            int n = namesMesh.getNumVertices();
            float nearestDistance = 0;
            ofVec2f nearestVertex;
            int nearestIndex = 0;
    
            ofVec2f mouse(mouseX, mouseY);
            for(int i = 0; i < n; i++) {
                ofVec3f cur = camera.worldToScreen(namesMesh.getVertex(i));
                float distance = cur.distance(ofVec2f(mouse));
                if(i == 0 || distance < nearestDistance) {
            nearestDistance = distance;
            nearestVertex = cur;
            nearestIndex = i;
                }
            }
    
    ofSetColor(ofColor::gray);
    ofDrawLine(nearestVertex, mouse);
    
    ofNoFill();
    ofSetColor(ofColor::yellow);
    ofSetLineWidth(2);
    ofDrawCircle(nearestVertex, 4);
    ofSetLineWidth(1);
    
    ofVec2f offset(10, -10);
    ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
            // DRAW NAMES CONTINUED
    
    
    //int n = namesMesh.getNumVertices();
        //cout << n << "\n";
    
    
    
    //cout << vbo.positionAttribute << "\n";

        //cout << n << "\n";
        //   cout <<  vbo.getTexCoordId() << "\n";
            //for(int i = 0; i < n; i++) {
                //camera.worldToScreen(asd);
                //ofVec3f cur = camera.worldToScreen(vbo.get(i));
            //    ofVec3f cur = camera.worldToScreen(vbo(i).x);
            //
            //    float distance = cur.distance(mouse);
            //    if(i == 0 || distance < nearestDistance) {
            //        nearestDistance = distance;
            //        nearestVertex = cur;
            //        nearestIndex = i;
            //    }
            //}
            //for (int u = 0; u < (int)points.size()-1; u++) {
            //    ofSetColor(255, 0, 0);
            //    ofDrawCircle(ofPoint(points[u].x, points[u].y), 10.0);
                //glPointSize(20.0);
                //vbo.draw(GL_POINTS, u, 10);
                //drawNames(ofVec2f(points[u].x, points[u].y), ofVec2f(ofGetMouseX(), ofGetMouseY()));
            //}
    
    
    
        ofDisableBlendMode();
        //ofPopMatrix();
        ofDisablePointSprites();
        ofDisableAlphaBlending();
    
        rightEyeFBO.end();
    
           /* */
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    rightEyeFBO.draw(0, 0);
    ofDisableBlendMode();
   
    //leftEyeFBO.draw(0, 0);
    //cout << leftEyeFBO.getWidth() << "\n";
    ofDisableAlphaBlending();
    ofSetColor(255, 255, 255);
    gui.draw();
}

void ofApp::exit() {

}

void ofApp::mouseReleased(int x, int y, int button) {
    
}

void ofApp::windowResized(int w, int h)
{
    initFBOs();
}

void ofApp::keyPressed(int key) {
    
    switch(key){
            
        case 's':
            //gui.toggleDraw();
            //if(gui.isOn()) ofShowCursor();
            //else ofHideCursor();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 't':
            camera.setOrientation(ofQuaternion(90.0, 0.0, 1.0, 0.0));
            break;
    
    }
}

double ofApp::drand()   /* uniform distribution, (0..1] */
{
    float a = (rand()+1.0)/(RAND_MAX+1.0);
    return a;
}

double ofApp::random_normal()  /* normal distribution, centered on 0, std dev 1 */
{
    float z = sqrt(-2*log(drand())) * cos(2*M_PI*drand());
    //cout << z << "\n";
    return z;
}

/*
void ofApp::drawName( const ofVec2f &mousePos, float power, float alpha ) {
    if( mDistToCam > 0.0f && mNameTex ){
        float per = constrain( 1.0f - mDistToCam * 0.0000375f, 0.0f, 1.0f );
        per *= per * per;
        
        Vec2f dirToMouse	= mScreenPos - mousePos;
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
}
*/

void ofApp::checkOSC() {
    
    while(receiver.hasWaitingMessages()){    // check for waiting messages
        ofxOscMessage m;                     // get the next message
        receiver.getNextMessage(m);
        
        string msg_string;
        msg_string = m.getAddress();
        msg_string += ": ";
        for(int i = 0; i < m.getNumArgs(); i++){
            // get the argument type
            msg_string += m.getArgTypeName(i);
            msg_string += ":";
            // display the argument - make sure we get the right type
            if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                msg_string += ofToString(m.getArgAsInt32(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                msg_string += ofToString(m.getArgAsFloat(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                msg_string += m.getArgAsString(i);
            }
            else{
                msg_string += "unknown";
            }
        }
        
        //cout << msg_string << "\n";
        
        // this part happens twice, because we receive ON and then OFF straight after
        
        if(m.getAddress() == "/accxyz"){
            //cout << m.getArgAsFloat(1) << "\n";
            
            delta = m.getArgAsFloat(1) * 2.0;
            //delta = delta; * 0.85 + deltaNEW*0.15;
            //cout << m.getArgAsFloat(0) << "\n";
            //if (m.getArgAsFloat(1) == 1 ) {
            //    filterCoordinates = !filterCoordinates;
            //    //cout << filterCoordinates << "\n";
            //    filterVisible =         false;
            //    filterXraySound =       false;
            //    filterComposition =     false;
            //}
        }
       /*
        if(m.getAddress() == "/1/fader1"){
            cout << m.getArgAsFloat(0) << "\n";
            //xSTR = ofToString(roundf(m.getArgAsFloat(0) * 10) / 10);
        }
        
        if(m.getAddress() == "/1/fader2"){
            cout << m.getArgAsFloat(0) << "\n";
            //ySTR = ofToString(roundf(m.getArgAsFloat(0) * 10) / 10);
        }
        
        if(m.getAddress() == "/1/fader3"){
            cout << m.getArgAsFloat(0) << "\n";
            //zSTR = ofToString(roundf(m.getArgAsFloat(0) * 10) / 10);
        }
        */
        /*
         else {
         // unrecognized message: display on the bottom of the screen
         string msg_string;
         msg_string = m.getAddress();
         msg_string += ": ";
         for(int i = 0; i < m.getNumArgs(); i++){
         // get the argument type
         msg_string += m.getArgTypeName(i);
         msg_string += ":";
         // display the argument - make sure we get the right type
         if(m.getArgType(i) == OFXOSC_TYPE_INT32){
         msg_string += ofToString(m.getArgAsInt32(i));
         }
         else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
         msg_string += ofToString(m.getArgAsFloat(i));
         }
         else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
         msg_string += m.getArgAsString(i);
         }
         else{
         msg_string += "unknown";
         }
         }
         
         // add to the list of strings to display
         //msg_strings[current_msg_string] = msg_string;
         //timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
         //current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
         // clear the next line
         //msg_strings[current_msg_string] = "";
         }*/
        
    }
}

void ofApp::drawNames(const ofVec2f &nearestVertex, const ofVec2f &mousePos) {
//    ofSetColor(ofColor::gray);
//    ofDrawLine(nearestVertex, mousePos);
//    ofNoFill();
//    ofSetColor(ofColor::yellow);
//    ofSetLineWidth(2);
//    ofDrawCircle(nearestVertex, 4);
//    ofSetLineWidth(1);
//    ofVec2f offset(10, -10);
//    ofDrawBitmapStringHighlight(ofToString("aga"), ofPoint(ofGetMouseX(), ofGetMouseY()));
    
    // DRAW NAMES
    //if( mRenderNames ){
    //    if( power < 0.5f ){
    //        gl::enableAlphaBlending();
    //    } else {
    //        gl::enableAdditiveBlending();
    //    }
    /*    gl::setMatricesWindow( getWindowSize(), true );
        
        BOOST_FOREACH( Star* &s, mNamedStars ){
            s->drawName( mMousePos, power * mScalePer, math<float>::max( sqrt( mScalePer ) - 0.1f, 0.0f ) );
        }*/
    //}

}

void ofApp::parseData( const string &path ) {
    
    string line;
    
    ofBuffer file = ofBufferFromFile(path);
    int i = 0;
    ofBuffer::Lines lines = file.getLines();
    
    for (const std::string& line : lines) {
        //std::cout << "LINE: " << line << std::endl;
        //if (i < 250) {
        createStar( line, i );
        ++i;
        //cout << line << "\n";
        //}
    }
    file.clear();
    
    
    //while( !myfile.eof() ){
    //    std::getline( myfile, line );
    //    createStar( line, i );
    //    ++i;
    //}
    
    //myfile.close();
    //} else std::cout << "Unable to open file";
}

void ofApp::createStar( const string &text, int lineNumber ) {
    
    int index = 0;
    int id;
    //float rad;
    //int type, subType;
    //string greekLetter;
    string name;
    //string regDate;
    //float xPos, yPos, zPos;
    //float appMag, absMag;
    //float colIndex;
    //std::string name;
    //std::string spectrum;
    //0   1       2     3        4            5     6               7     8     9
    //id, radius, type, subType, greekLetter, name, registeredDate, xPos, yPos, zPos
    if (lineNumber > 0) { // exclude the 1st ite which is decription
        vector<string> splitItems = ofSplitString(text, ",");
        for(auto i = splitItems.begin(); i != splitItems.end(); ++i) {
            //std::cout << *i << ' ';
            
            if( index == 0 ){
                id = ofToInt(*i);
            } else if( index == 1 ){
                if ( (*i).length() > 1 ) { name = (*i); } else { name = ""; }
            }
            /*} else if( index == 3 ){
                subType = ofToInt(*i);
            } else if( index == 4 ){
                greekLetter = (*i);
            } else if( index == 5 ){
                if ( (*i).length() > 1 ) { name = (*i); } else { name = ""; }
            } else if( index == 6 ){
                regDate = (*i);
            } else if( index == 7 ){
                xPos = ofToFloat(*i);
            } else if( index == 8 ){
                yPos = ofToFloat(*i);
            } else if( index == 9 ){
                zPos = ofToFloat(*i);
            }*/
            index ++;
        }
        cout << id << " | " << name << "\n";
        //cout << rad << " " << type << " " << subType << " " << greekLetter << " " << name << " " << regDate << " " << xPos << " " << yPos << " " << zPos << "\n";
        
        // ofVec3f pos = ofVec3f( xPos, yPos, zPos );
        // THIS FEELS WRONG. ASK ABOUT THE RIGHT WAY TO DO THIS.
        Star star ( id , name );
            //Star star (pos, rad, type, subType, greekLetter, name, regDate);
        mStars.push_back( star );
    }
    
    //if( appMag < 6.0f || name.length() > 1 ){
    //mBrightStars.push_back( star );
    //} else {
    //mFaintStars.push_back( star );
    //}
    
    //if( name.length() > 1 ){
    //mNamedStars.push_back( star );
    
    //if( name == "Sol" || name == "Sirius" || name == "Vega" || name == "Gliese 581" ){
    //mTouringStars.push_back( star );
    //std::cout << "ADDED TOURING STAR: " << star->mName << " " << star->mPos << std::endl;
    // }
    //}
    // }
    
    
    
    
    /* this part works */
    
    //ofBuffer buff = ofBufferFromFile("star_data/MOCK_DATA.csv");
    
    //for(auto line: buff.getLines()){
    //    vector<string> splitItems = ofSplitString(line, ",");
    //    for(auto i = splitItems.begin(); i != splitItems.end(); ++i) {
    //        std::cout << *i << ' ';
    //    }
    //    cout << "\n";
    //    for (vector<string>::const_iterator i = splitItems.begin(); i != splitItems.end(); ++i) {
    //    std::cout << *i << ' ';
    //    }
    //}
    
    /* this part works end */
}

