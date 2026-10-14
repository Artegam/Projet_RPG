#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <vector>

#include <iobyte.h>

namespace image {
  class BMP {
    private:
      struct Header;
      struct Informations;

      Header * header;
      Informations * infos;
      std::vector<unsigned char> pixels;

    public:
      BMP();
      bool load(const char* filename);
      void analyseHeader(const unsigned char* header);
      unsigned int width();
      unsigned int height();
      const unsigned char * data();
  };

  struct BMP::Header {
      short sign;            // 2 octets - equivalent short
      unsigned int filesize; // 4 octets - equivalent unsigned int
      unsigned int dataStart;// pointeur vers les données de l'image
  };

  struct BMP::Informations {
      unsigned int size;
      unsigned int width;
      unsigned int height;
      unsigned int imageSize;
      short plans;
      short color_deep;
      unsigned int compress_method;
      unsigned int raw_size;
      unsigned int h_resolution; //ppm = pixels par metre
      unsigned int v_resolution; //ppm
      unsigned int palette_colors;
      unsigned int important_colors;
  };
}

#endif
