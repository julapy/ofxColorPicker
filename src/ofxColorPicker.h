//
//  ofxColorPicker.h
//  ofxColorPicker
//
//  Created by Lukasz Karluk on 13/08/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"

#define	COLOR_PICKER_DEFAULT_WIDTH 150
#define	COLOR_PICKER_DEFAULT_HEIGHT 300
#define	COLOR_WHEEL_RES 200

struct PolCord {
	float angle;
	float radius;
};

class ofxColorPicker {

public :
	
    ofxColorPicker();
    ~ofxColorPicker();
    
    void addListeners();
    void removeListeners();
    
    void update();
    
    void setPos(float x, float y);
    void setSize(float x, float y, float w, float h);
    
    void draw() const;
    
    float getWidth() const;
    float getHeight() const;
    
    void show();
    void hide();
    void toggleShow();
    bool isShowing();
    
    void enable();
    void disable();
    
    void setColor(const ofColor& c);
    const ofColor & getColor();
    
    void setColorScale(float value);
    float getColorScale();
    
    void setColorRadius(float value);
    float getColorRadius();
    
    void setColorAngle(float value);
    float getColorAngle();
    
    void touchDown(float x, float y, int id);
    void touchMoved(float x, float y, int id);
    void touchUp(float x, float y, int id);
	
private :

    void init();
    bool updateDimensions(int x, int y, int w, int h);
    bool updateColorWheelSize();
    bool updateColorWheelColor();
    bool updateColorScaleBarSize();
    bool updateColorScaleBarColor();
    
    void update(ofEventArgs &e);
    void draw(ofEventArgs &e);
    void exit(ofEventArgs &e);
    
    void updateColor();
    void updateColorScale();
    void updateColorPoint();
    
    void drawBackground() const;
    void drawColorWheel() const;
    void drawColorPoint() const;
    void drawColorScaleBar() const;
    void drawColorRect() const;
    
    ofColor getCircularColor(float angle, float radius, float scale) const;
    const ofPoint getPoint(float a, float r) const;
    PolCord getPolarCoordinate(float x, float y) const;
    
    ofxMSAInteractiveObject colorWheel;
    ofxMSAInteractiveObject colorScaleBar;
    
    bool bVisible;
    bool bEnabled;
    bool bAddedListeners;
    
    ofColor color;
    ofPoint colorPoint;
    float colorScale;
    float colorRadius;
    float colorAngle;
    bool bColorScaleChanged;
    
    ofPoint mousePoint;
    
    ofRectangle dimensions;
    int colorWheelRadius;
    int colorWheelPad;
    
    ofRectangle rectBackground;
    ofRectangle rectColorWheel;
    ofRectangle rectColorScaleBar;
    ofRectangle rectColorBox;
    
    ofMesh meshColorWheel;
    ofMesh meshColorScaleBar;
};
