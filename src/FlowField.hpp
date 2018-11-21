#ifndef FLOWFIELD_H
#define FLOWFIELD_H


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
    for (std::size_t i = 0; i < numRows; i++) {
      for (std::size_t j = 0; j < numColumns; j++) {
        field.at(j * numColumns + i) = glm::vec2(ofNoise(i, j), ofNoise(i, j));
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



#endif /* FLOWFIELD_H */
