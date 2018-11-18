#pragma once

#include <random>

#include "ofMain.h"

template<class Vector>
float heading(const Vector &v) {
  return std::atan2(v.y, v.x);
}
template<class Vector>
glm::vec2 limit(const Vector &v, float maximum) {
  auto l = glm::length(v);
  if (l >= maximum) {
    return glm::normalize(v) * maximum;
  }
  return v;
}

constexpr double fib(unsigned int n) {
  if (n == 0) return 0;
  if (n == 1 || n == 2) return 1;
  return fib(n-1) + fib(n-2);
}

constexpr double approxGolden(unsigned int n) {
  return fib(n) / fib(n-1);
}

constexpr double goldenRatio = approxGolden(25);

class Timer {
  std::uint64_t startTime;
  std::uint64_t duration;
  bool running = true;

public:
  Timer(std::uint64_t duration_) : duration(duration_) {
  }
  
  void start() {
    startTime = ofGetElapsedTimeMillis();
    running = true;
  }

  void stop() {
    running = false;
  }

  bool isRunning() const {
    return running;
  }

  bool isElapsed() {
    auto elapsed = ofGetElapsedTimeMillis() - startTime;
    if (elapsed >= duration) {
      startTime = ofGetElapsedTimeMillis();
      return true && running;
    } else {
      return false;
    }
  }


};


class OneShotTimer {

  std::uint64_t startTime;
  std::uint64_t duration;
  std::function<void()> callback;
  bool triggered{false};

public:
  void setTimeout(std::uint64_t duration_, std::function<void()> fun) {
    startTime = ofGetElapsedTimeMillis();
    duration = duration_;
    callback = fun;
    triggered = false;
  }

  void update() {
    if (triggered) return;
    if ((ofGetElapsedTimeMillis() - startTime) >= duration) {
      triggered = true;
      callback();
    }
  }

  void reset() {
    triggered = false;
    startTime = ofGetElapsedTimeMillis();
  }
};

template<typename T>
std::uniform_real_distribution<T> make_real_distribution(T start, T end, bool includeEnd = false) {
  std::uniform_real_distribution<T> distribution{start, includeEnd ? std::nextafter(end, std::numeric_limits<T>::max()) : end};
  return distribution;
}

template<typename T>
std::uniform_int_distribution<T> make_int_distribution(T start, T end, bool includeEnd = false) {
  std::uniform_int_distribution<T> distribution{start, includeEnd ? end + 1 : end};
  std::cout << "int\n";
  return distribution;
}

struct Circle {
  glm::vec2 position;
  double length;
  ofColor color;
  Circle(glm::vec2 pos, double l, ofColor color_) : position(pos), length(l), color(color_) { 
  }
};


class FlowField {
  std::vector<glm::vec2> field;
  std::size_t numColumns;
  std::size_t numRows;

public:

  std::size_t getWidth() const {
    return numColumns;
  }

  std::size_t getHeight() const {
    return numRows;
  }

  FlowField(std::size_t width, std::size_t height)
    : field(height*width), numColumns(height), numRows(width) {

    for (int i = 0; i < numRows; i++) {
      for (int j = 0; j < numColumns; j++) {
        field.at(j * numColumns + i) = glm::vec2(ofNoise(i, j), ofNoise(i, j));
      }
    }
  }

  void update() {

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> val = make_real_distribution(-1.f, 1.f, true);

    for (int i = 0; i < numRows; i++) {
      for (int j = 0; j < numColumns; j++) { 
        field.at(i * numColumns + j) = glm::vec2(val(gen), val(gen));
      }
    }
  }

  void setFlow(glm::vec2 flow, std::size_t x, std::size_t y) {
    field.at(numColumns * x + y) = flow;
  }

  glm::vec2 getFlow(std::size_t x, std::size_t y) const {
    return field.at(numColumns * x + y);
  }

};


template<class C>
glm::vec2 align(std::vector<C> const& boids) {
  glm::vec2 result = std::accumulate(std::next(std::begin(boids)),
                                     std::end(boids),
                                     boids[0].getVelocity(),
                                     [](auto a, auto b){ return a + b.getVelocity(); });
  return result / boids.size();
}

template<class C>
glm::vec2 separate(std::vector<C> const& boids) {
  return {};
}

template<class C>
glm::vec2 cohesion(std::vector<C> const& boids) {
  return {};
}

struct Boid {
  glm::vec2 acceleration;
  glm::vec2 velocity;
  glm::vec2 position;
  
  Boid(glm::vec2 startPosition) : position(startPosition) {

  }

  glm::vec2 getVelocity() const {
    return velocity;
  }

  void applyForce(glm::vec2 force) {
    acceleration += force;
  }

  void update() {
    velocity += acceleration;

    velocity = limit(velocity, 5);

    position += velocity;

    if (position.x < 0) position.x = ofGetWindowWidth();
    if (position.x > ofGetWindowWidth()) position.x = 0;
    if (position.y < 0) position.y = ofGetWindowHeight();
    if (position.y > ofGetWindowHeight()) position.y = 0;

    acceleration *= 0;
  }

  void flock(std::vector<Boid> const& boids) {
    //glm::vec2 separation = separate(boids);
    glm::vec2 alignment = align(boids);
    //glm::vec2 cohesion = cohesion(boids);

    //separation *= 1.5;
    alignment *= 1.0;
    //cohesion *= 1.0;

    //applyForce(separation);
    applyForce(alignment);
    //applyForce(cohesion);
  }

  void draw() {

    glm::vec2 p1{0, -10};
    glm::vec2 p2{30, 0};
    glm::vec2 p3{0, 10};

    ofPushMatrix();
    ofTranslate(position);
    ofRotateRad(heading(velocity));
    ofDrawTriangle(p1, p2, p3);
    ofPopMatrix();
  }

  glm::vec2 getPosition() {
    return position;
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


  std::vector<Boid> boids;

  FlowField flowField{100, 100};

  ofFbo fbo;

  Boid pokemon{glm::vec2(200.0, 200.0)};

  void drawField(FlowField const& field, float width, float height) {
    float xResolution = width / field.getWidth();
    float yResolution = height / field.getHeight();
    for (int i = 0; i < field.getWidth(); i++) {
      for (int j = 0; j < field.getHeight(); j++) {
        glm::vec2 flow = field.getFlow(i, j);
        glm::vec2 pos = glm::vec2(i * xResolution + xResolution / 2,
                                  j * yResolution + yResolution / 2);
        ofDrawLine(pos, pos);
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

