#ifndef BOIDS_H
#define BOIDS_H

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

template<class C>
glm::vec2 seek(C const& agent, glm::vec2 target, float maxSpeed) {
  glm::vec2 desired = target - agent.getPosition();
  return glm::normalize(desired) * maxSpeed;
}

template<class Vector>
Vector steer(Vector target, Vector location, float maxForce = 1.0) {
  return (target - location) * maxForce;
}

template<class C>
glm::vec2 alignement(C const& agent, std::vector<C> const& agents, float radius, float maxSpeed) {
  int count = 0;
  glm::vec2 result;
  for (auto const& a : agents) {
    if (glm::distance(agent.getPosition(),a.getPosition()) > 0
        && glm::distance(agent.getPosition(),a.getPosition()) < radius) {
      count++;
      result += a.getVelocity();
    }
  }

  if (count > 0) {
    result /= count;
    result = glm::normalize(result) * maxSpeed;
    return result;
  } else {
    return glm::vec2(0);
  }
}

template<class C>
glm::vec2 separate(C const& agent, std::vector<C> const& agents, float radius, float maxSpeed) {
  int count = 0;
  glm::vec2 result;

  for (auto const& a : agents) {
    float d = glm::distance(agent.getPosition(),a.getPosition());
    if (d > 0 && d < radius * 2) {
      glm::vec2 diff = agent.getPosition() - a.getPosition();
      diff = glm::normalize(diff);
      diff /= glm::distance(agent.getPosition(), a.getPosition());
      count++;
      result += diff;
    }
  }

  if (count > 0) {
    result /= count;
    result = glm::normalize(result) * maxSpeed;
    return result;
  } else {
    return glm::vec2(0);
  }
}

template<class C>
glm::vec2 cohesion(C const& agent, std::vector<C> const& agents, float radius, float maxSpeed) {
  int count = 0;
  glm::vec2 result;

  for (auto const& a : agents) {
    float d = glm::distance(agent.getPosition(),a.getPosition());
    if (d > 0 && d < radius) {
      count++;
      result += a.getPosition();
    }
  }

  if (count > 0) {
    result /= count;
    return seek(agent, result, maxSpeed);
  } else {
    return glm::vec2(0);
  }

}

struct AutonomousAgent {
  glm::vec2 acceleration;
  glm::vec2 velocity{10.0, 0.0};
  glm::vec2 position;

  AutonomousAgent(glm::vec2 startPosition) : position(startPosition) {

  }

  glm::vec2 getVelocity() const {
    return velocity;
  }

  glm::vec2 getPosition() const {
    return position;
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


#endif /* BOIDS_H */
