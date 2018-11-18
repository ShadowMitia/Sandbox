#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

  /*timer.setTimeout(250, [&](){
                      points.emplace_back(glm::vec2{x(generator) * goldenRatio, y(generator) * goldenRatio}, length(generator) * goldenRatio, ofColor(color(generator), color(generator), color(generator)));
                      timer.reset();

                      for (auto& circle : points) {
                        circle.length = length(generator)*goldenRatio;
                        circle.color = ofColor(color(generator), color(generator), color(generator));
                      }

                        });
  */

  ofSetFullscreen(true);
  ofSetCircleResolution(200);

  for (std::size_t i = 0; i < 50; i++) {
    boids.emplace_back(glm::vec2(x(generator), y(generator)));
  }
  ofBackground(0);
  fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);
  fbo.begin();
  ofClear(255,255,255, 0);
  fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
  ofSetWindowTitle(ofToString(ofGetFrameRate()));
  timer.update();

  //for (auto& boid : boids) {
    //boid.applyForce(computeCurl(boid.getPosition().x, boid.getPosition().y) * 10);
    //boid.flock(boids);
    //boid.update();
  //}

  pokemon.applyForce(flowField.getFlow(static_cast<int>(pokemon.position.x) % flowField.getWidth(),
                                       static_cast<int>(pokemon.position.y) % flowField.getHeight()));
  pokemon.update();

  fbo.begin();
  ofBackground(0);
  /*for (auto& boid : boids) {
    boid.draw();
    }*/

  flowField.update();

  ofSetColor(ofColor::red);
  drawField(flowField, ofGetWindowWidth(), ofGetWindowHeight());
  ofSetColor(ofColor::white);
  pokemon.draw();
  fbo.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
  
  // for (auto const& circle : points) {
  //   ofSetColor(circle.color);
  //   ofDrawCircle(circle.position, circle.length);
  // }

  fbo.draw(0, 0);

  ofSetColor(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
  pokemon.applyForce(seek(glm::vec2{x, y}, pokemon.position, pokemon.velocity, 5, 5));
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
