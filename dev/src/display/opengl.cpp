#include "display/opengl.h"

GLuint OpenGL::textureID = 0; //[ASC] Initialisation

OpenGL::OpenGL() {
  // Initialisation de FreeGLUT
  //glutInit(&argc, argv);
  int nb = 1;
  char* args[1];
  args[0]=(char*)"mon programme...";
  glutInit(&nb, args);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Affichage Image - FreeGLUT");
}

void OpenGL::initTexture(std::string filename) {
  int width, height;
  std::vector<unsigned char> pixels;

  // Chargement avec notre propre fonction
  // ATTENTION : Enregistrez votre image au format "BMP 24-bits"
  image::BMP bmp;
  if (bmp.load(filename.c_str(), width, height, pixels)) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // /!\ IMPORTANT : Le format BMP stocke les couleurs dans l'ordre Bleu, Vert, Rouge (BGR)
    // Heureusement, OpenGL possède le format natif GL_BGR
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels.data());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    logger << "Image BMP chargee : " << width << "x" << height << eol();
  }
}

// Fonction de rendu
void OpenGL::fct_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Activer l'utilisation des textures 2D
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, OpenGL::textureID);

  // Dessiner un rectangle texturé qui remplit l'écran
  glBegin(GL_QUADS);
  // glTexCoord2f(U, V) -> coordonnée sur l'image (0 à 1)
  // glVertex2f(X, Y)   -> position à l'écran (-1 à 1)
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f); // Bas Gauche
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);  // Bas Droite
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);   // Haut Droite
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);  // Haut Gauche
  glEnd();

  glDisable(GL_TEXTURE_2D);
  glutSwapBuffers();
}

void OpenGL::display() {
  glutDisplayFunc(OpenGL::fct_display);
  glutMainLoop();
}
