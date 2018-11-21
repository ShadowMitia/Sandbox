#ifndef TIMERS_H
#define TIMERS_H


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


#endif /* TIMERS_H */
