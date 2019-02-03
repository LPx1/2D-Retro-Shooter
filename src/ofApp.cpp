#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//set up for player emitter
	ofSetVerticalSync(true);
	score = 0;
	enemyDir = 1;
	player.name = "Player";
	moveDir = MoveStop;

	//Path file to load sound files from
	laserSound.load("sounds/shoot2.mp3");
	gameMusic.load("sounds/MHtheme.mp3");
	explosion.load("sounds/explosion1.mp3");

	//Path file to load image files
	background.load("images/NightSky.jpg");
	playerImg.load("images/ship.png");
	bullet.load("images/laser.png");
	player.setImage(playerImg);
	player.setChildImage(bullet);
//	player.drawable = true;
	enemy.load("images/squid.png");
	enemy2.load("images/metroid.png");

	gameMusic.setLoop(true);
	gameMusic.play();

	//gui
	gui.setup();
	gui.add(rateofFire.setup("Rate of Fire", 5, 1, 10));
	gui.add(xAxis.setup("X",0, -1000, 1000));
	gui.add(yAxis.setup("Y", -400, -1000, 1000));
	gui.add(lifeSpan.setup("Bullet LifeSpan", 3000, 10, 10000));

	gui.add(EnRate.setup("Enemy spawn rate", 0.5, 0, 10));
	gui.add(EnLife.setup("Enemy Lifespan", 5, .1, 10));
	gui.add(EnVelocity.setup("Enemy Velocity", ofVec3f(0, 150, 0),
											ofVec3f(-1000, -1000, -1000),
											ofVec3f(1000, 1000, 1000)));

	//Physic Forces
	gravityForce = new GravityForce(ofVec3f(0, -10, 0));
	radialForce = new ImpulseRadialForce(50.0);
	turbForce = new TurbulenceForce(ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1));
//	turbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));

	system.addForce(turbForce);
	xForce.lifespan = 1000000000;
	xForce.position = (ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));
	system.add(xForce);
	

	//player.sys = gun;
	player.width = 50;
	player.height = 50;
	player.velocity = ofVec3f(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));
	player.setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));
	player.speed = 250;

	numEmitters = 5; 
	float margin = 25;
	float emitterWidth = 25;
	ofPoint winsize = ofGetWindowSize();
	float spacing = (winsize.x - emitterWidth - margin * 2) / (numEmitters - 1);
	float x = emitterWidth / 2 + margin;

	for (int i = 0; i < numEmitters; i++) {
		Emitter *emit = new Emitter(new SpriteSystem());
		emit->setPosition(ofVec3f(x, 0, 0));
		emit->drawable = false;                // make emitter itself invisible
		emit->setChildImage(enemy);
		x += spacing;
		spawners.push_back(emit);
		emit->start();
	}

	//Overwrites the image 
	spawners[1]->setChildImage(enemy2);
	spawners[3]->setChildImage(enemy2);
}

//--------------------------------------------------------------
void ofApp::update(){

	if (startGame) {
		player.update();
		player.setRate(rateofFire);
		player.setVelocity(ofVec3f(xAxis, yAxis, 0)); //Changes Velocity of the bullets 
		player.setLifespan(lifeSpan);

		//When gun fires, emits sounds
		if (player.sound) {
			laserSound.play();
		}

		float dist = player.speed * 1 / ofGetFrameRate();
		ofVec3f dir;
		ofRectangle r = ofGetWindowRect();

		switch (moveDir)
		{
		case MoveUp:
			dir = ofVec3f(0, -dist, 0);
			break;
		case MoveDown:
			dir = ofVec3f(0, dist, 0);
			break;
		case MoveLeft:
			dir = ofVec3f(-dist, 0, 0);
			break;
		case MoveRight:
			dir = ofVec3f(dist, 0, 0);
			break;
		}

		if (player.trans.x < 0) {
			player.setPosition(ofVec2f(0, player.trans.y));
		}
		else if ((player.trans.x + player.width) > 1024) {
			player.setPosition(ofVec2f((1024 - player.width), player.trans.y));
		}

		if (player.trans.y < 0) {
			player.setPosition(ofVec2f(player.trans.x, 0));
		}
		else if ((player.trans.y + player.height) > 768) {
			player.setPosition(ofVec2f(player.trans.x, (768 - player.height)));
		}

		//Turbelence Forces
		float x = ofRandom(-1.5, 1.5);
		float y = ofRandom(-1.5, 1.5);

		ofVec3f forces = ofVec3f(x, y, 0);
		player.trans += (dir + forces) ;

		for (int i = 0; i < spawners.size(); i++) {
			Emitter *emitter = spawners[i];
			emitter->setRate(EnRate);
			emitter->setLifespan(EnLife * 1000);    // convert to milliseconds 
			emitter->setVelocity(ofVec3f(EnVelocity->x, EnVelocity->y, EnVelocity->z));
			emitter->update();
			emitter->sys->update();
		}

		//After elapsed time modify the velocity vector of the enemies
		if (ofGetElapsedTimeMillis() - velocityTimeChange > (10000)) {
			enemyDir *= -1;
			EnVelocity.setup("Enemy Velocity", ofVec3f(enemyDir * 30, 200, 0),
				ofVec3f(-1000, -1000, -1000),
				ofVec3f(1000, 1000, 1000));
			velocityTimeChange = ofGetElapsedTimeMillis();
		}


		checkCollisions();
		system.update();
		forces = ofVec3f(0, 0, 0); //Reset forces and re-add
		
	}
}


void ofApp::moveSprite(MoveDir dir) {
	moveDir = dir;
}

void ofApp::stopSprite() {
	moveDir = MoveStop;
}

void ofApp::checkCollisions() {
	// remove all sprites emitted within a radius equal to the max
	// distance sprite can travel in one frame
	for (int i = 0; i < spawners.size(); i++) {
		for (int j = 0; j < player.sys->sprites.size(); j++) {
			spawners[i]->sys->removeNear(player.sys->sprites[j].trans, spawners[i]->maxDistPerFrame());
			if (spawners[i]->sys->erased) {
				//explosion.play();
				score++;
//				printf_s("%d", score);
			}
			//	spawners[i]->sys->removeNear(player.trans + 10, spawners[i]->maxDistPerFrame());
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	background.draw(0, 0, 1024, 768);
	

	if (startGame) {
	//	gui.draw();
		player.draw(); // Draws Player sprite
		xForce.draw();

		string scoreText;
		scoreText += "Score: " + std::to_string(score);
		ofDrawBitmapStringHighlight(scoreText, glm::vec2(900,30), ofColor::black, ofColor::greenYellow);

		//Draw out all emitters 
		for (int i = 0; i < spawners.size(); i++) {
			spawners[i]->draw();
		}
		//	gun.sys->draw(); 
		ofDrawCircle(ofVec2f(20, 20 ), 5);
	}
	else {
		ofDrawBitmapStringHighlight("PRESS SPACE TO START", glm::vec2(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2),
			ofColor::black, ofColor::greenYellow);
		ofDrawBitmapStringHighlight("SQUID INVADERS", glm::vec2(ofGetWindowWidth() / 2 + 20, ofGetWindowHeight() / 2 + 20),
			ofColor::black, ofColor::greenYellow);
	
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (startGame) {
		switch (key) {
		case ' ':
			player.start();
			break;
		case 'F':
		case 'f':
			ofToggleFullscreen();
			break;
		case OF_KEY_RIGHT:
			moveSprite(MoveRight);
			break;
		case OF_KEY_LEFT:
			moveSprite(MoveLeft);
			break;
		case OF_KEY_UP:
			moveSprite(MoveUp);
			break;
		case OF_KEY_DOWN:
			moveSprite(MoveDown);
			break;
		}

	}


	//Requires player to hit space bar to begin the game otherwise controls wont
	//be unlocked.
	if (!startGame && (key == ' ')) {
		startGame = true;
		cout << "Start!" << endl; 
		
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case ' ':
		//gun.stop();
		player.stop();
		break;
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		stopSprite();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
