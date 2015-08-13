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

void ofApp::setup()
{
	ofSetFrameRate( frameRate = 60 );
	ofSetVerticalSync( true );
	
	colorPicker0.setColorRadius( 1.0 );
	colorPicker1.setColorRadius( 1.0 );
	
	colorPicker0.setColorAngle( 0.5 );
	colorPicker1.setColorAngle( 0.83 );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void ofApp::update()
{
	float ang;
	ang += frameRate * 0.00002;

	colorPicker1.setColorAngle( colorPicker1.getColorAngle() + ang );
	
	colorPicker0.update();
	colorPicker1.update();
	
	rect.setCornerColor( colorPicker0.getColor(), 0 );
	rect.setCornerColor( colorPicker0.getColor(), 1 );
	rect.setCornerColor( colorPicker1.getColor(), 2 );
	rect.setCornerColor( colorPicker1.getColor(), 3 );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void ofApp::draw()
{
	rect.draw();
	
	//--

	int x, y, w, h, g;

	w = 150;
	h = 300;
	x = 20;
	
	g = (int)( ( ofGetHeight() - h * 2 ) / 3 );		// gap.
	y = g;
	
	colorPicker0.draw( x, y, w, h );
	
	y = y + h + g;
	
	colorPicker1.draw( x, y, w, h );
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void ofApp::keyPressed(int key)
{

}

void ofApp::keyReleased(int key)
{

}

void ofApp::mouseMoved(int x, int y )
{

}

void ofApp::mouseDragged(int x, int y, int button)
{

}

void ofApp::mousePressed(int x, int y, int button)
{

}

void ofApp::mouseReleased(int x, int y, int button)
{

}

void ofApp::windowResized(int w, int h)
{

}

