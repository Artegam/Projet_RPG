#ifndef IMAGEPARSER_H
#define IMAGEPARSER_H

#include <iostream>
#include <fstream>
#include <vector>

namespace imageparser {
  class BMP {
    public:
      bool load(const char* filename, int& width, int& height, std::vector<unsigned char>& pixels);
  };
}

#endif
