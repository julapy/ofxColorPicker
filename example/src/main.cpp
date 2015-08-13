//
//  main.cpp
//  ofxColorPicker
//
//  Created by Lukasz Karluk on 13/08/2015.
//
//

#include "ofMain.h"
#include "ofApp.h"

int main() {
    
    ofGLFWWindowSettings windowSettings;
//    windowSettings.setGLVersion(3, 2);
    windowSettings.width = 1280;
    windowSettings.height = 720;
    windowSettings.windowMode = OF_WINDOW;
    
    ofCreateWindow(windowSettings);
    
    ofRunApp(new ofApp());
}
