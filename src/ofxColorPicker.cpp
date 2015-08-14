//
//  ofxColorPicker.cpp
//  ofxColorPicker
//
//  Created by Lukasz Karluk on 13/08/2015.
//
//

#include "ofxColorPicker.h"

ofxColorPicker::ofxColorPicker() {
    init();
}

ofxColorPicker::~ofxColorPicker() {
    removeListeners();
    colorWheel.disableMouseEvents();
    colorScaleBar.disableMouseEvents();
}

//////////////////////////////////////////////
//	INIT.
//////////////////////////////////////////////

void ofxColorPicker::init() {
    
    dimensions.x = 0;
    dimensions.y = 0;
    dimensions.width = 0;
    dimensions.height = 0;
    
    colorScale = 1.0;
    colorRadius = 0;
    colorAngle = 0;
    bColorScaleChanged = false;
    
    bVisible = true;
    bEnabled = true;
    bAddedListeners = false;
    
    mousePoint = getPoint(colorAngle, colorRadius);
    
    setSize(0, 0, COLOR_PICKER_DEFAULT_WIDTH, COLOR_PICKER_DEFAULT_HEIGHT);
    
    colorWheel.enableMouseEvents();
    colorScaleBar.enableMouseEvents();
    
    show();
    enable();
}

void ofxColorPicker::exit(ofEventArgs &e) {
    //
}

void ofxColorPicker::addListeners() {
    if(bAddedListeners == true) {
        return;
    }
    
    ofAddListener(ofEvents().update, this, &ofxColorPicker::update);
    ofAddListener(ofEvents().draw, this, &ofxColorPicker::draw);
    ofAddListener(ofEvents().exit, this, &ofxColorPicker::exit);
    
    bAddedListeners = true;
}


void ofxColorPicker::removeListeners() {
    if(bAddedListeners == false) {
        return;
    }
    
    ofRemoveListener(ofEvents().update, this, &ofxColorPicker::update);
    ofRemoveListener(ofEvents().draw, this, &ofxColorPicker::draw);
    ofRemoveListener(ofEvents().exit, this, &ofxColorPicker::exit);
    
    bAddedListeners = false;
}

//////////////////////////////////////////////
//	RENDER RELATED METHODS.
//////////////////////////////////////////////

void ofxColorPicker::setPos(float x, float y) {
    updateDimensions(x, y, dimensions.width, dimensions.height);
}

void ofxColorPicker::setSize(float x, float y, float w, float h) {
    updateDimensions(x, y, w, h);
}

bool ofxColorPicker::updateDimensions(int x, int y, int w, int h) {
    
    bool bPositionChange = false;
    bPositionChange = bPositionChange || (dimensions.x != x);
    bPositionChange = bPositionChange || (dimensions.y != y);
    
    bool bSizeChange = false;
    bSizeChange = bSizeChange || (dimensions.width != w);
    bSizeChange = bSizeChange || (dimensions.height != h);
    
    bool bChanged = bPositionChange || bSizeChange;
    
    if(bChanged == false) {
        return false;
    }
    
    dimensions.x = x;
    dimensions.y = y;
    dimensions.width = w;
    dimensions.height = h;
    
    int minWH = MIN(w, h);
    
    colorWheelRadius = (int)(minWH * 0.5 * 0.9);
    colorWheelPad = (int)((minWH - colorWheelRadius * 2) * 0.5);
    
    rectBackground.x = (int)(dimensions.x);
    rectBackground.y = (int)(dimensions.y);
    rectBackground.width = (int)(dimensions.width);
    rectBackground.height = (int)(dimensions.height);
    
    rectColorWheel.x = (int)(dimensions.x + colorWheelPad);
    rectColorWheel.y = (int)(dimensions.y + colorWheelPad);
    rectColorWheel.width = (int)(colorWheelRadius * 2);
    rectColorWheel.height = (int)(colorWheelRadius * 2);
    
    rectColorScaleBar.width = (int)(dimensions.width - colorWheelPad * 2);
    rectColorScaleBar.height = (int)(rectColorScaleBar.width * 0.2);
    rectColorScaleBar.x = (int)(dimensions.x + colorWheelPad);
    rectColorScaleBar.y	= (int)(dimensions.y + colorWheelPad * 2 + colorWheelRadius * 2);
    
    rectColorBox.x = (int)(dimensions.x + colorWheelPad);
    rectColorBox.y = (int)(rectColorScaleBar.y + rectColorScaleBar.height + colorWheelPad);
    rectColorBox.width = (int)(dimensions.width - colorWheelPad * 2);
    rectColorBox.height = (int)(dimensions.height - (rectColorBox.y - dimensions.y) - colorWheelPad);
    
    colorWheel.setPos(rectColorWheel.x, rectColorWheel.y);
    colorWheel.setSize(rectColorWheel.width, rectColorWheel.height);
    
    colorScaleBar.setPos(rectColorScaleBar.x, rectColorScaleBar.y);
    colorScaleBar.setSize(rectColorScaleBar.width, rectColorScaleBar.height);
    
    if(bSizeChange == true) {
        updateColorWheelSize();
        updateColorScaleBarSize();
    }
    
    return true;
}

bool ofxColorPicker::updateColorWheelSize() {

    meshColorWheel.clear();
    meshColorWheel.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    int colorWheelX = colorWheelRadius;
    int colorWheelY = colorWheelRadius;
    
    for(int i=0; i<COLOR_WHEEL_RES+1; i++) {
        int j = i % COLOR_WHEEL_RES;
        float p = j / (float)COLOR_WHEEL_RES;
        float a = p * TWO_PI;
        
        ofVec3f p0, p1;
        p0.x = colorWheelX + cos(-a) * colorWheelRadius;
        p0.y = colorWheelY + sin(-a) * colorWheelRadius;
        p1.x = colorWheelX;
        p1.y = colorWheelY;
        
        meshColorWheel.addVertex(p0);
        meshColorWheel.addVertex(p1);
        
        meshColorWheel.addColor(ofColor::white);
        meshColorWheel.addColor(ofColor::white);
    }
    
    updateColorWheelColor();
    
    return true;
}

bool ofxColorPicker::updateColorWheelColor() {

    for(int i=0; i<COLOR_WHEEL_RES+1; i++) {
        int j = i % COLOR_WHEEL_RES;
        float p = j / (float)COLOR_WHEEL_RES;
        float a = p * TWO_PI;
        
        ofColor c0 = getCircularColor(a * RAD_TO_DEG, 1.0, colorScale);
        ofColor c1 = ofColor::white;
        
        meshColorWheel.setColor(i*2+0, c0);
        meshColorWheel.setColor(i*2+1, c1);
    }

    return true;
}

bool ofxColorPicker::updateColorScaleBarSize() {

    int padding = 2;
    int w = rectColorScaleBar.width - padding * 2;
    int h = rectColorScaleBar.height - padding * 2;
    
    meshColorScaleBar.clear();
    meshColorScaleBar.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    meshColorScaleBar.addVertex(ofVec3f(0, 0));
    meshColorScaleBar.addVertex(ofVec3f(w, 0));
    meshColorScaleBar.addVertex(ofVec3f(w, h));
    meshColorScaleBar.addVertex(ofVec3f(0, h));
    
    meshColorScaleBar.addColor(ofColor::white);
    meshColorScaleBar.addColor(ofColor::white);
    meshColorScaleBar.addColor(ofColor::white);
    meshColorScaleBar.addColor(ofColor::white);
    
    updateColorScaleBarColor();
}

bool ofxColorPicker::updateColorScaleBarColor() {

    ofColor c0 = ofColor::black;
    ofColor c1 = getCircularColor(colorAngle, colorRadius, 1.0);
    
    meshColorScaleBar.setColor(0, c0);
    meshColorScaleBar.setColor(1, c1);
    meshColorScaleBar.setColor(2, c1);
    meshColorScaleBar.setColor(3, c0);
}

void ofxColorPicker::updateColorPoint() {
    int x = rectColorWheel.x;
    int y = rectColorWheel.y;
    int w = rectColorWheel.width;
    int h = rectColorWheel.height;
    
    colorPoint = getPoint(colorAngle, colorRadius);
    colorPoint.x += x;
    colorPoint.y += y;
    
    colorPoint.x = ofClamp( colorPoint.x, x, x + w );
    colorPoint.y = ofClamp( colorPoint.y, y, y + h );
}

float ofxColorPicker::getWidth() const {
    return dimensions.width;
}

float ofxColorPicker::getHeight() const {
    return dimensions.height;
}

//////////////////////////////////////////////
//	EVENT HANDLERS.
//////////////////////////////////////////////

void ofxColorPicker::update(ofEventArgs &e) {
	update();
}

void ofxColorPicker::update() {
	updateColor();
	updateColorScale();
    updateColorPoint();
    
    if(bColorScaleChanged == true) {
        bColorScaleChanged = false;
        updateColorWheelColor();
    }
	
	ofColor colorNew = getCircularColor(colorAngle, colorRadius, colorScale);
    
    bool bColorChanged = false;
    bColorChanged = bColorChanged || (color.r != colorNew.r);
    bColorChanged = bColorChanged || (color.g != colorNew.g);
    bColorChanged = bColorChanged || (color.b != colorNew.b);
    
    color = colorNew;
    
    if(bColorChanged == true) {
        updateColorScaleBarColor();
    }
}

void ofxColorPicker::updateColor() {
	if(colorWheel.isMouseDown() == false) {
        return;
	}
    
    mousePoint.x = colorWheel.getMouseX() - rectColorWheel.x;
    mousePoint.y = colorWheel.getMouseY() - rectColorWheel.y;
    
    PolCord pc;
    pc = getPolarCoordinate(mousePoint.x, mousePoint.y);
    
    colorAngle	= pc.angle;
    colorRadius	= pc.radius;
    colorRadius = MIN(1, MAX(0, colorRadius)); // constrain to the radius of color circle.
}

void ofxColorPicker::updateColorScale() {
	if(colorScaleBar.isMouseDown() == false) {
        return;
	}
    
    int relX = colorScaleBar.getMouseX() - rectColorScaleBar.x;
    float scale = MIN(1, MAX(0, relX / (float)(colorWheelRadius * 2)));
    setColorScale(scale);
}

//////////////////////////////////////////////
//	COLOR PICKER SETUP.
//////////////////////////////////////////////

void ofxColorPicker::setColorScale(float value) {
    if(colorScale == value) {
        return;
    }
    colorScale = value;
    bColorScaleChanged = true;
}

float ofxColorPicker::getColorScale() {
    return colorScale;
}

//------

void ofxColorPicker::setColorRadius (float value) {
    colorRadius = value;
}

float ofxColorPicker::getColorRadius() {
    return colorRadius;
}

//------

void ofxColorPicker::setColorAngle(float value) {
    colorAngle = value * 360;
}

float ofxColorPicker::getColorAngle() {
    return colorAngle / 360.0;
}

//////////////////////////////////////////////
//	VISIBLE.
//////////////////////////////////////////////

void ofxColorPicker::show() {
    bVisible = true;
    colorWheel.enabled = bVisible && bEnabled;
    colorScaleBar.enabled = bVisible && bEnabled;
}

void ofxColorPicker::hide() {
    bVisible = false;
    colorWheel.enabled = bVisible && bEnabled;
    colorScaleBar.enabled = bVisible && bEnabled;
}

void ofxColorPicker::toggleShow() {
    if(bVisible == true) {
        hide();
    } else {
        show();
    }
}

bool ofxColorPicker::isShowing() {
    return bVisible;
}

//////////////////////////////////////////////
//	ENABLE / DISABLE.
//////////////////////////////////////////////

void ofxColorPicker::enable() {
    bEnabled = true;
    colorWheel.enabled = bVisible && bEnabled;
    colorScaleBar.enabled = bVisible && bEnabled;
}

void ofxColorPicker::disable() {
    bEnabled = false;
    colorWheel.enabled = bVisible && bEnabled;
    colorScaleBar.enabled = bVisible && bEnabled;
}

//////////////////////////////////////////////
//	TOUCH INPUT.
//////////////////////////////////////////////

void ofxColorPicker::touchDown(float x, float y, int id) {
    ofMouseEventArgs e;
    e.x = x;
    e.y = y;
    e.button = id;
    
    colorWheel._mousePressed(e);
    colorScaleBar._mousePressed(e);
}

void ofxColorPicker::touchMoved(float x, float y, int id) {
    ofMouseEventArgs e;
    e.x = x;
    e.y = y;
    e.button = id;
    
    colorWheel._mouseDragged(e);
    colorScaleBar._mouseDragged(e);
}

void ofxColorPicker::touchUp(float x, float y, int id) {
    ofMouseEventArgs e;
    e.x = x;
    e.y = y;
    e.button = id;
    
    colorWheel._mouseReleased(e);
    colorScaleBar._mouseReleased(e);
}

//////////////////////////////////////////////
//	DISPLAY.
//////////////////////////////////////////////

void ofxColorPicker::draw(ofEventArgs &e) {
    draw();
}

void ofxColorPicker::draw() const {
    if(bVisible == false) {
        return;
    }
    
    ofPushStyle();
    
    drawBackground();
    drawColorWheel();
    drawColorPoint();
    drawColorScaleBar();
    drawColorRect();
    
    ofPopStyle();
}

void ofxColorPicker::drawBackground() const {
    int x = rectBackground.x;
    int y = rectBackground.y;
    int w = rectBackground.width;
    int h = rectBackground.height;
    
    int bx, by;
    
    bx = by = 0;
    ofFill();
    ofSetColor(113, 113, 113);
    ofDrawRectangle(x + bx, y + by, w - bx * 2, h - by * 2);
    
    bx = by = 1;
    ofSetColor(232, 232, 232);
    ofDrawRectangle(x + bx, y + by, w - bx * 2, h - by * 2);
}

void ofxColorPicker::drawColorWheel() const {
    ofSetSmoothLighting(true);
    
    ofPushMatrix();
    ofTranslate(rectColorWheel.x, rectColorWheel.y);
    meshColorWheel.draw();
    ofPopMatrix();
}

void ofxColorPicker::drawColorPoint() const {
    ofFill();
    ofSetColor(ofColor::black);
    ofDrawCircle(colorPoint.x, colorPoint.y, 4);
    ofSetColor(ofColor::white);
    ofDrawCircle(colorPoint.x, colorPoint.y, 2);
    
    ofNoFill();
    ofSetColor(ofColor::black);
    ofDrawCircle(colorPoint.x, colorPoint.y, 4);
    ofSetColor(ofColor::white);
    ofDrawCircle(colorPoint.x, colorPoint.y, 2);
}

void ofxColorPicker::drawColorScaleBar() const {
    int x = rectColorScaleBar.x;
    int y = rectColorScaleBar.y;
    int w = rectColorScaleBar.width;
    int h = rectColorScaleBar.height;
    
    ofFill();
    
    ofSetColor(149, 255);
    ofDrawRectangle(x, y, w, h);
    
    ofSetColor(ofColor::white);
    ofDrawRectangle(x + 1, y + 1, w - 2, h - 2);
    
    int rx, ry, rw, rh;
    rx = x + 2;
    ry = y + 2;
    rw = w - 4;
    rh = h - 4;
    
    ofSetSmoothLighting(true);
    
    ofPushMatrix();
    ofTranslate(rx, ry);
    meshColorScaleBar.draw();
    ofPopMatrix();
    
    //--
    
    int bx, by;
    int cw = 3;
    int cx = colorScale * (rw - cw);
    
    //-- grey rect for handle. bx and by are border values.
    
    bx = 2;
    by = 4;
    
    ofSetColor(149, 255);
    ofDrawRectangle(rx + cx - bx, ry - by, cw + bx * 2, rh + by * 2);
    
    //-- white rect for handle. bx and by are border values.
    
    bx = 1;
    by = 3;
    
    ofSetColor(ofColor::white);
    ofDrawRectangle(rx + cx - bx, ry - by, cw + bx * 2, rh + by * 2);
}

void ofxColorPicker::drawColorRect() const {
    ofFill();
    
    int x = rectColorBox.x;
    int y = rectColorBox.y;
    int w = rectColorBox.width;
    int h = rectColorBox.height;
    
    int bx, by;
    
    bx = by = 0;
    ofSetColor(149, 255);
    ofDrawRectangle(x + bx, y + by, w - bx * 2, h - by * 2);
    
    bx = by = 1;
    ofSetColor(ofColor::white);
    ofDrawRectangle(x + bx, y + by, w - bx * 2, h - by * 2);
    
    bx = by = 2;
    ofSetColor(color.r, color.g, color.b, color.a);
    ofDrawRectangle(x + bx, y + by, w - bx * 2, h - by * 2);
}

//////////////////////////////////////////////
//	COLOR MAPPING.
//////////////////////////////////////////////

void ofxColorPicker::setColor(const ofColor & c) {
    float da;
    
    if((c.r == c.g) && (c.r == c.b)) {
        colorScale	= c.r / 255.0;
        colorRadius	= 0;
        colorAngle	= 0;
    }
    
    if((c.r > c.g) && (c.r > c.b)) { // red.
        colorScale = c.r / (float)255.0;
        
        if(c.g > c.b) {     // between 0-60
            colorRadius = 1 - c.b / (float)(255.0 * colorScale);
            da = (c.g / (float)(255 * colorScale) - (1 - colorRadius)) / colorRadius;
            colorAngle = da * 60;
        } else {            // between 300-360
            colorRadius = 1 - c.g / (float)(255.0 * colorScale);
            da = (c.b / (float)(255 * colorScale) - (1 - colorRadius)) / colorRadius;
            colorAngle = 360 - da * 60;
        }
    }
    
    if((c.g > c.r) && (c.g > c.b)) { // green.
        colorScale = c.g / (float)255.0;
        
        if(c.r > c.b) {     // between 60-120
            colorRadius = 1 - c.b / (float)(255.0 * colorScale);
            da = (c.r / (float)(255 * colorScale) - (1 - colorRadius)) / colorRadius;
            colorAngle = 120 - da * 60;
        } else {            // between 120-180
            colorRadius = 1 - c.r / (float)(255.0 * colorScale);
            da = (c.b / (float)(255 * colorScale) - (1 - colorRadius)) / colorRadius;
            colorAngle = da * 60 + 120;
        }
    }
    
    if((c.b > c.r) && (c.b > c.g)) { // blue.
        colorScale = c.b / (float)255.0;
        
        if(c.g > c.r) {     // between 180-240
            colorRadius = 1 - c.r / (float)(255.0 * colorScale);
            da = (c.g / (float)(255 * colorScale) - (1 - colorRadius)) / colorRadius;
            colorAngle = 240 - da * 60;
        } else {            // between 240-300
            colorRadius = 1 - c.g / (float)(255.0 * colorScale);
            da = (c.r / (float)(255 * colorScale) - (1 - colorRadius)) / colorRadius;
            colorAngle = da * 60 + 240;
        }
    }
    
    update();
}

const ofColor & ofxColorPicker::getColor() {
    return color;
}

ofColor ofxColorPicker::getCircularColor(float angle, float radius, float scale) const {
    radius = MIN(1.0, MAX(0.0, radius));
    
    angle -= floor(angle / 360) * 360;
    if(angle < 0) {
        angle += 360;
    }
    
    float da;
    
    ofColor c;
    
    if(angle < 60) {
        
        da  = angle / 60;
        c.r = (int)(255 * scale);
        c.g = (int)(255 * (da + (1 - da) * (1 - radius)) * scale);
        c.b = (int)(255 * (1 - radius) * scale);
        c.a = 255;
        
    } else if(angle < 120) {
        
        da  = (120 - angle) / 60;
        c.r = (int)(255 * (da + (1 - da) * (1 - radius)) * scale);
        c.g = (int)(255 * scale);
        c.b = (int)(255 * (1 - radius) * scale);
        c.a = 255;
        
    } else if(angle < 180) {
        
        da  = 1 - (180 - angle) / 60;
        c.r = (int)(255 * (1 - radius) * scale);
        c.g = (int)(255 * scale);
        c.b = (int)(255 * (da + (1 - da) * (1 - radius)) * scale);
        c.a = 255;
        
    } else if(angle < 240) {
        
        da  = (240 - angle) / 60;
        c.r = (int)(255 * (1 - radius) * scale);
        c.g = (int)(255 * (da + (1 - da) * (1 - radius)) * scale);
        c.b = (int)(255 * scale);
        c.a = 255;
        
    } else if(angle < 300) {
        
        da  = 1 - (300 - angle) / 60;
        c.r = (int)(255 * (da + (1 - da) * (1 - radius)) * scale);
        c.g = (int)(255 * (1 - radius) * scale);
        c.b = (int)(255 * scale);
        c.a = 255;
        
    } else if(angle <= 360) {
        
        da  = (360 - angle) / 60;
        c.r = (int)(255 * scale);
        c.g = (int)(255 * (1 - radius) * scale);
        c.b = (int)(255 * (da + (1 - da) * (1 - radius)) * scale);
        c.a = 255;
    }
    
    return c;
}

const ofPoint ofxColorPicker::getPoint(float a, float r) const {
    float cx = colorWheelRadius;
    float cy = colorWheelRadius;
    
    ofPoint p;
    p.x = cx * r * cos(-a * DEG_TO_RAD) + cx;
    p.y = cy * r * sin(-a * DEG_TO_RAD) + cy;
    
    return p;
}

PolCord ofxColorPicker::getPolarCoordinate(float x, float y) const {
    float cx = colorWheelRadius;
    float cy = colorWheelRadius;
    
    float px = x - cx;						// point x from center.
    float py = y - cy;						// point x from center.
    float pl = sqrt(px * px + py * py);     // point length from center.
    float pa = atan2(px, py);				// point angle around center.
    
    pa *= RAD_TO_DEG;
    pa -= 90;
    pl /= cx;
    
    PolCord pc;
    pc.angle = pa;
    pc.radius = pl;
    
    return pc;
}
