#define STB_IMAGE_IMPLEMENTATION
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>

// Identifiant de la texture OpenGL
GLuint textureID;

// Fonction maison pour charger un fichier BMP 24-bits
bool loadBMP(const char* filename, int& width, int& height, std::vector<unsigned char>& pixels) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Impossible d'ouvrir le fichier BMP : " << filename << std::endl;
    return false;
  }

  // Lire l'en-tête du fichier (54 octets)
  unsigned char header[54];
  file.read(reinterpret_cast<char*>(header), 54);

  // Vérifier la signature "BM"
  if (header[0] != 'B' || header[1] != 'M') {
    std::cerr << "Ce n'est pas un fichier BMP valide." << std::endl;
    return false;
  }

  // Extraire la largeur, la hauteur et la taille des données depuis l'en-tête
  width  = *reinterpret_cast<int*>(&header[18]);
  height = *reinterpret_cast<int*>(&header[22]);
  int imageSize = *reinterpret_cast<int*>(&header[34]);

  // Si la taille de l'image n'est pas spécifiée, on la calcule
  if (imageSize == 0) imageSize = width * height * 3;

  // Allouer le vecteur pour stocker les pixels
  pixels.resize(imageSize);

  // Lire les pixels bruts du fichier
  file.read(reinterpret_cast<char*>(pixels.data()), imageSize);
  file.close();

  return true;
}

void initTexture() {
  int width, height;
  std::vector<unsigned char> pixels;

  // Chargement avec notre propre fonction
  // ATTENTION : Enregistrez votre image au format "BMP 24-bits"
  if (loadBMP("images/image.bmp", width, height, pixels)) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // /!\ IMPORTANT : Le format BMP stocke les couleurs dans l'ordre Bleu, Vert, Rouge (BGR)
    // Heureusement, OpenGL possède le format natif GL_BGR
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels.data());

    std::cout << "Image BMP chargee : " << width << "x" << height << std::endl;
  }
}

// Fonction de rendu
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Activer l'utilisation des textures 2D
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID);

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


int main(int argc, char** argv) {
  // Initialisation de FreeGLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Affichage Image - FreeGLUT");

  initTexture();

  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}

//===================================================================

