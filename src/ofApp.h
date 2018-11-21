#pragma once

#include <random>

#include "ofMain.h"

#include "Timers.hpp"
#include "FlowField.hpp"
#include "Boids.hpp"



constexpr double fib(unsigned int n) {
  if (n == 0) return 0;
  if (n == 1 || n == 2) return 1;
  return fib(n-1) + fib(n-2);
}

constexpr double approxGolden(unsigned int n) {
  return fib(n) / fib(n-1);
}

constexpr double goldenRatio = approxGolden(25);

template<typename T>
std::uniform_real_distribution<T> make_real_distribution(T start, T end, bool includeEnd = false) {
  std::uniform_real_distribution<T> distribution{start, includeEnd ? std::nextafter(end, std::numeric_limits<T>::max()) : end};
  return distribution;
}

template<typename T>
std::uniform_int_distribution<T> make_int_distribution(T start, T end, bool includeEnd = false) {
  std::uniform_int_distribution<T> distribution{start, includeEnd ? end + 1 : end};
  return distribution;
}

struct Circle {
  glm::vec2 position;
  double length;
  ofColor color;
  Circle(glm::vec2 pos, double l, ofColor color_) : position(pos), length(l), color(color_) { 
  }
};


class ofApp : public ofBaseApp{

public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  unsigned int windowWidth = ofGetWindowWidth();
  unsigned int windowHeight = ofGetWindowHeight();

  OneShotTimer timer;
  Timer periodicTimer = Timer(2000);

  std::vector<Circle> points;
  std::random_device rd{};
  std::mt19937 generator{rd()};
  std::uniform_real_distribution<double> distribution{0.0, std::nextafter(1, std::numeric_limits<double>::max())};
  std::uniform_real_distribution<double> length{5.0, std::nextafter(50.0, std::numeric_limits<double>::max())};
  std::uniform_real_distribution<double> x{0.0, std::nextafter(windowWidth, std::numeric_limits<double>::max())};
  std::uniform_real_distribution<double> y{0.0, std::nextafter(windowHeight, std::numeric_limits<double>::max())};
  std::uniform_int_distribution<int> color{0, 256};

  unsigned int block_width = 100;

  glm::vec2 computeCurl(float x, float y) {
    float eps = 1.0;
    float n1 = ofNoise(x, y + eps);
    float n2 = ofNoise(x, y - eps);
    float a = (n1 - n2) / (2 * eps);

    float n3 = ofNoise(x + eps, y);
    float n4 = ofNoise(x - eps, y);
    float b = (n3 - n4) / (2 * eps);

    return glm::vec2{a, -b};
  }


  std::vector<AutonomousAgent> boids;

  FlowField flowField{10, 10};

  ofFbo fbo;

  AutonomousAgent pokemon{glm::vec2(200.0, 200.0)};

  void drawField(FlowField const& field, float width, float height) {
    float xResolution = width / field.getWidth();
    float yResolution = height / field.getHeight();
    for (std::size_t i = 0; i < field.getWidth(); i++) {
      for (std::size_t j = 0; j < field.getHeight(); j++) {
        glm::vec2 flow = field.getFlow(i, j);
        glm::vec2 pos = glm::vec2(i * xResolution + xResolution / 2,
                                  j * yResolution + yResolution / 2);
        ofDrawLine(pos, pos + flow * 5);
      }
    }
  }




  glm::vec2 seek(glm::vec2 target, glm::vec2 location, glm::vec2 velocity, float maxSpeed, float maxForce) {
    glm::vec2 desired = target - location;
    desired = glm::normalize(desired);
    desired *= maxSpeed;
    glm::vec2 steer = desired - velocity;
    steer = limit(steer, maxForce);
    return steer;
  }

};

