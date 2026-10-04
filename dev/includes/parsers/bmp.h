#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <vector>

namespace image {
  class BMP {
    private:
//structure head ???
      short sign; // 2 octets
      unsigned int filesize;
      unsigned int dataStart;
      unsigned int r(const unsigned char* data, unsigned int nboctets, bool little_endian = true);
      void r(short& result, const unsigned char* data, bool little_endian = true);
      void r(int& result, const unsigned char* data, bool little_endian = true);
      void r(unsigned int& result, const unsigned char* data, bool little_endian = true);
      bool is_little_endian();
    public:
      bool load(const char* filename, int& width, int& height, std::vector<unsigned char>& pixels);
      void analyseHeader(const unsigned char* header, int& width, int& height, int& imageSize);
  };
}

#endif
