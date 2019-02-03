#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
//#include "ofTimer.h"

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;

// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D
//
class BaseObject {
public:
	BaseObject();
	ofVec2f trans, scale;
	float	rot;
	bool	bSelected;
	void setPosition(ofVec3f p) { trans = p; }
};

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	float age();
	void setImage(ofImage);
	float speed;    //   in pixels/sec
	ofVec3f velocity; // in pixels/sec
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	string name;
	bool haveImage;
	float width, height;
};

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem {
public:
	SpriteSystem();
	void add(Sprite);
	void remove(int);
	void update();
	void draw();
	void removeNear(ofVec3f point, float dist);
	vector<Sprite> sprites;
	void test();
	bool erased;
	ofSoundPlayer explo;
	int dead;
	

};

//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public BaseObject {
public:
	Emitter(SpriteSystem *);
	void draw();
	void start();
	void stop();
	void setLifespan(float);
	void setVelocity(ofVec3f);
	void setChildImage(ofImage);
	void setImage(ofImage);
	void setRate(float);
	void update();
	float maxDistPerFrame();
	SpriteSystem *sys;
	float rate;
	float speed;    //   in pixels/sec
	ofVec3f velocity;
	ofVec3f vel;
	ofVec3f accelaration;
	float lifespan;
	float birthtime; //added
	bool started;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
	bool sound;
	string name;
	Sprite sprite;
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void stopSprite();
	void checkCollisions();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void moveSprite(MoveDir);

	ofxFloatSlider rateofFire; //rate of fire of gun
	ofxFloatSlider xAxis;
	ofxFloatSlider yAxis; //controls of the velocity 
	ofxPanel gui;
	ofxFloatSlider lifeSpan;
	bool bHide;

	ofxFloatSlider EnRate;
	ofxFloatSlider EnLife;
	ofxVec3Slider EnVelocity;
	bool imageLoaded;

	//Creates instances of player and gun emitter to shoot out sprites
//	Sprite player;
	Emitter player = Emitter(new SpriteSystem());

	vector<Emitter *> spawners;
	int numEmitters;
	int enemyDir;
	float velocityTimeChange;
	int score;


	//Start points for sprite player
	ofVec3f start_point;
	ofVec3f end_point;
	MoveDir moveDir;
	bool startGame;

	//Sounds 
	ofSoundPlayer laserSound;
	ofSoundPlayer gameMusic;
	ofSoundPlayer explosion;

	//Images 	
	ofImage background;
	ofImage enemy;
	ofImage enemy2;
	ofImage playerImg;
	ofImage bullet;


	//Physics and Forces 
	TurbulenceForce *turbForce;
	GravityForce *gravityForce;
	ImpulseRadialForce *radialForce;

	ParticleEmitter emitter;
	ParticleSystem system;
	Particle xForce;
};
