#ifndef OPENGL_H
#define OPENGL_H
#define STB_IMAGE_IMPLEMENTATION
#include <GL/freeglut.h>
#include "parsers/bmp.h"
#include "logger.h"

#include <iostream>
#include <string>


class OpenGL {
  private:
    Logger logger;

    static void fct_display();

  public:
    // Identifiant de la texture OpenGL
    static GLuint textureID;

    OpenGL();
    void initTexture(std::string filename);
    void display();
};

#endif
