#include "display/opengl.h"

int main(int argc, char** argv) {
  OpenGL d;
  d.initTexture("images/image.bmp");
  d.display();
  return 0;
}
