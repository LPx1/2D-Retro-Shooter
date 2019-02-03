#include "ofApp.h"

Sprite::Sprite() {
	width = 25;
	height = 50;
	birthtime = 0;
	//velocity = ofVec3f(400, 300);
//	birthtime = ofGetElapsedTimeMillis(); 
//	lifespan = GetCurrentTime() - birthtime;
}

//Draws sprite 
void Sprite::draw()
{
//	ofVec2f pos = ofVec2f(400, 300);
	ofSetColor(255, 255, 255, 255);
	if (haveImage) {
		image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
//		ofSetColor(0, 0, 200);
//		ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
	}
	else {
		ofDrawRectangle(trans, width, height);
		ofSetColor(255, 255, 255, 255);
	}

}

//Returns the time sprite has been alive
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//If image is not set, it is replaced by a rectangle
void Sprite::setImage(ofImage i) {
	image = i;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}