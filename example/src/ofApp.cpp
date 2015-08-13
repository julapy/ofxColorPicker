//
//  ofApp.cpp
//  ofxColorPicker
//
//  Created by Lukasz Karluk on 13/08/2015.
//
//

#include "ofApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void ofApp::setup() {
    
	ofSetFrameRate(60);
	
	colorPicker0.setColorRadius(1.0);
    colorPicker0.setColorAngle(0.5);

    colorPicker1.setColorRadius(1.0);
	colorPicker1.setColorAngle(0.83);
    
    meshGradient.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

    meshGradient.addVertex(ofVec3f(0, 0));
    meshGradient.addVertex(ofVec3f(ofGetWidth(), 0));
    meshGradient.addVertex(ofVec3f(ofGetWidth(), ofGetHeight()));
    meshGradient.addVertex(ofVec3f(0, ofGetHeight()));
    
    meshGradient.addColor(ofColor::white);
    meshGradient.addColor(ofColor::white);
    meshGradient.addColor(ofColor::white);
    meshGradient.addColor(ofColor::white);
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void ofApp::update() {

    float angle = colorPicker1.getColorAngle();
	angle += ofGetLastFrameTime() * 0.03;

	colorPicker1.setColorAngle(angle);
	
	colorPicker0.update();
	colorPicker1.update();
    
    ofColor colorTop = colorPicker0.getColor();
    ofColor colorBot = colorPicker1.getColor();
    
    meshGradient.setColor(0, colorTop);
    meshGradient.setColor(1, colorTop);
    meshGradient.setColor(2, colorBot);
    meshGradient.setColor(3, colorBot);
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void ofApp::draw() {

    meshGradient.draw();
	
    int x = 20;
    int y = 0;
    int w = 150;
    int h = 300;
	int g = (int)((ofGetHeight() - h * 2) / 3); // gap.
	y = g;
	
	colorPicker0.draw(x, y, w, h);
	
	y = y + h + g;
	
	colorPicker1.draw(x, y, w, h);
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void ofApp::keyPressed(int key) {
    //
}

void ofApp::keyReleased(int key) {
    //
}

void ofApp::mouseMoved(int x, int y ) {
    //
}

void ofApp::mouseDragged(int x, int y, int button) {
    //
}

void ofApp::mousePressed(int x, int y, int button) {
    //
}

void ofApp::mouseReleased(int x, int y, int button) {
    //
}

void ofApp::windowResized(int w, int h) {
    //
}

