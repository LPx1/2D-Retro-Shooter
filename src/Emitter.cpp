#include "ofApp.h"

Emitter::Emitter(SpriteSystem *x) {
	sys = x;
	birthtime = ofGetElapsedTimeMillis();
	lifespan = ofGetElapsedTimeMillis() - birthtime;
	haveChildImage = false;
	haveImage = false;
	
}

void Emitter::draw() {
//	ofSetColor(255, 255, 255, 255);
//	image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
	if (drawable) {

		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
			width = image.getWidth();
			height = image.getHeight();
			//ofSetColor(0, 0, 200);
			//ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
		}
		else {
			ofSetColor(255, 255, 255, 255);
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
			}
	}
	sys->draw();
}

void Emitter ::start() {
	started = true;
}

void Emitter::stop() {
	started = false;
}

void Emitter::setLifespan(float num) {
	lifespan = num;
}

void Emitter::setVelocity(ofVec3f var) {
	velocity = var;
}

void Emitter::setChildImage(ofImage pic) {
	childImage = pic;
	haveChildImage = true;
}

void Emitter::setImage(ofImage picture) {
	image = picture;
	haveImage = true;
	drawable = true; //When emitter has an image of its own draw it
	width = image.getWidth();
	height = image.getHeight();
}

void Emitter::setRate(float r) {
	rate = r;
}

void Emitter::update() {

	if (started && ((ofGetElapsedTimeMillis() - lastSpawned) > (1000.0/rate))) {
		if (haveChildImage) sprite.setImage(childImage);
		sprite.birthtime = ofGetElapsedTimeMillis(); //When bullet was spawned
		sprite.width = 20;
		sprite.height = 20;
		sprite.trans = ofVec2f(trans.x + 15, trans.y + 30);
		sprite.velocity = velocity;
		sprite.lifespan = lifespan;
		sys->add(sprite);
		lastSpawned = ofGetElapsedTimeMillis();
		sound = true;

	}
	else
		sound = false;

	sys->update();
}

float Emitter::maxDistPerFrame() {
	return (velocity.length() * 10) / ofGetFrameRate();
}

