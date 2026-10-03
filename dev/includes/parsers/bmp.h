#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <vector>

namespace image {
  class BMP {
    public:
      bool load(const char* filename, int& width, int& height, std::vector<unsigned char>& pixels);
  };
}

#endif
