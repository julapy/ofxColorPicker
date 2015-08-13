//
//  main.cpp
//  ofxColorPicker
//
//  Created by Lukasz Karluk on 13/08/2015.
//
//

#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main( ){
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280,720, OF_WINDOW);
	ofRunApp( new ofApp());
}
