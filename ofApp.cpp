#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->box2d.init();
	this->box2d.setGravity(0, 15);
	this->box2d.createBounds();
	this->box2d.setFPS(30);
	this->box2d.registerGrabbing();

	this->fbo.allocate(ofGetWidth(), ofGetHeight());

}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->circles.size() - 1; i > -1; i--) {
		this->circles_life[i] -= 3.5;

		if (this->circles_life[i] < 0) {
			this->circles[i].get()->destroy();
			this->circles.erase(this->circles.begin() + i);
			this->circles_life.erase(this->circles_life.begin() + i);
			this->circles_color.erase(this->circles_color.begin() + i);
		}
	}

	this->box2d.update();

	this->fbo.begin();
	ofClear(0);

	for (int i = 0; i < this->circles.size(); i++) {
		vector<ofVec2f> near_point;

		for (int j = i; j < this->circles.size(); j++) {
			float distance = this->circles[i].get()->getPosition().distanceSquared(this->circles[j].get()->getPosition());
			if (distance < 50 * 50) {
				near_point.push_back(this->circles[j].get()->getPosition());
			}
		}

		ofSetColor(this->circles_color[i], 192);
		if (near_point.size() > 2) {
			ofBeginShape();
			for (int i = 0; i < near_point.size(); i++) {
				ofVertex(near_point[i].x, near_point[i].y);
			}
			ofEndShape(true);
		}
		else {
			continue;
		}

	}

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	this->fbo.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	float radius = 10;
	for (int i = 0; i < 5; i++) {
		float color_value = ofRandom(256);
		for (int deg = 0; deg < 360; deg += 10) {
			float tmp_x = radius * cos(deg * DEG_TO_RAD);
			float tmp_y = radius * sin(deg * DEG_TO_RAD);

			this->circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
			this->circles.back().get()->setPhysics(3.0, 0.5, 0.1);
			this->circles.back().get()->setup(this->box2d.getWorld(), x + tmp_x, y + tmp_y, 4);
			this->circles.back().get()->addForce(ofVec2f(tmp_x, tmp_y), 6.0);

			ofColor circle_color;
			circle_color.setHsb(color_value, 255, 255);
			this->circles_color.push_back(circle_color);

			this->circles_life.push_back(255);
		}

		radius += 3;
	}
}

//--------------------------------------------------------------
int main() {
	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}