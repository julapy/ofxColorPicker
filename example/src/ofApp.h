//
//  ofApp.h
//  ofxColorPicker
//
//  Created by Lukasz Karluk on 13/08/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxColorPicker.h"
#include "ColorRect.h"

class ofApp : public ofBaseApp
{
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    ofxColorPicker colorPicker0;
    ofxColorPicker colorPicker1;
    ofMesh meshGradient;
};