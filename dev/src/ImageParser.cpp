#include "ImageParser.h"

// Fonction maison pour charger un fichier BMP 24-bits
bool imageparser::BMP::load(const char* filename, int& width, int& height, std::vector<unsigned char>& pixels) {
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

  //[ASC] reorder the pixels
  std::vector<unsigned char> line;
  std::vector<unsigned char> tmp = pixels;
  pixels.clear();
  line.clear();

  for(int i = 0; i <= height; i++) {
    std::vector<unsigned char>::iterator bol = tmp.begin();
    std::vector<unsigned char>::iterator eol = tmp.begin();
    if(i > 0)
      advance(bol, (i-1)*width*3); // les trois couleurs sont codees sur 24 bits
    advance(eol, i*width*3);
    for(std::vector<unsigned char>::iterator it = eol; it != bol; it=it-3) {
      line.emplace(line.begin(), *it);
      line.emplace(line.begin(), *(it+1));
      line.emplace(line.begin(), *(it+2));
    }
    pixels.insert(pixels.begin(), line.begin(), line.end());
  }

  return true;
}

