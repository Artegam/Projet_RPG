#include "parsers/bmp.h"


//[ASC] operations sur 4 octets donc 32 bits
unsigned int image::BMP::r(const unsigned char* data, unsigned int nboctets, bool little_endian) {
  //initialisation
  unsigned int res = 0x00000000; // 4 octets

  for (int idx = nboctets-1; idx>=0; idx--)
    (little_endian) ? res |= data[idx] << 8*idx : res |= data[nboctets-1-idx] << 8*idx;

  return res;
}

void image::BMP::r(short& result, const unsigned char* data, bool little_endian) { result = r(data, 2, little_endian); }
void image::BMP::r(int& result, const unsigned char* data, bool little_endian) { result = r(data, 4, little_endian); }
void image::BMP::r(unsigned int& result, const unsigned char* data, bool little_endian) { result = r(data, 4, little_endian); }

bool image::BMP::is_little_endian() {
  unsigned int num = 1;
  // On convertit l'adresse de l'entier en pointeur sur char (1 octet)
  char* c = (char*)&num;
  // Si le premier octet vaut 1, c'est du little-endian
  return (*c == 1);
}

void image::BMP::analyseHeader(const unsigned char* header, int& width, int& height, int& imageSize) {
  r(sign, &header[0], false);
  //sign = (header[0] << 8) | header[1]; //ordre standart

  // Extraire la largeur, la hauteur et la taille des données depuis l'en-tête
  r(filesize, &header[2]);
  r(dataStart, &header[10]);
  r(width, &header[18]);
  r(height, &header[22]);
  r(imageSize, &header[34]);

  printf("sign: %x\n", sign);
  printf("filesize: %d octets\n", filesize);
  printf("data start pointer: %.4x\n", dataStart);
  printf("width: %x\n", width);
  printf("height: %x\n", height);

  // Si la taille de l'image n'est pas spécifiée, on la calcule
  if (imageSize == 0) imageSize = width * height * 3;
}

// Fonction maison pour charger un fichier BMP 24-bits
bool image::BMP::load(const char* filename, int& width, int& height, std::vector<unsigned char>& pixels) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Impossible d'ouvrir le fichier BMP : " << filename << std::endl;
    return false;
  }

  // Lire l'en-tête du fichier (54 octets)
  int imageSize = 0;
  unsigned char header[54];

  file.read(reinterpret_cast<char*>(header), 54);
  analyseHeader(header, width, height, imageSize);
  // Vérifier la signature "BM"
  if (sign != 0x424D) {
    std::cerr << "Ce n'est pas un fichier BMP valide." << std::endl;
    return false;
  }

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
      line.emplace(line.begin(), 0x0); //[ASC] canal Alpha
      line.emplace(line.begin(), *it);
      line.emplace(line.begin(), *(it+1));
      line.emplace(line.begin(), *(it+2));
    }
    pixels.insert(pixels.begin(), line.begin(), line.end());
  }

  return true;
}

