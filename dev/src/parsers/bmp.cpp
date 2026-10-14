#include "parsers/bmp.h"

image::BMP::BMP() {
  header = new Header();
  infos = new Informations();
  infos->imageSize = 0;
}

void image::BMP::analyseHeader(const unsigned char* _start) {
  header->sign = iobyte::mem::read<short>(&_start[0], false);

  // Extraire la largeur, la hauteur et la taille des données depuis l'en-tête
  header->filesize = iobyte::mem::read<unsigned int>(&_start[2]);
  header->dataStart = iobyte::mem::read<unsigned int>(&_start[10]);
  infos->width = iobyte::mem::read<unsigned int>(&_start[18]);
  infos->height = iobyte::mem::read<unsigned int>(&_start[22]);
  infos->imageSize = iobyte::mem::read<unsigned int>(&_start[34]);

  printf("sign: %x\n", header->sign);
  printf("filesize: %d octets\n", header->filesize);
  printf("data start pointer: %.4x\n", header->dataStart);
  printf("width: %x\n", infos->width);
  printf("height: %x\n", infos->height);

  // Si la taille de l'image n'est pas spécifiée, on la calcule
  if (infos->imageSize == 0) infos->imageSize = infos->width * infos->height * 3;
}

// Fonction maison pour charger un fichier BMP 24-bits
bool image::BMP::load(const char* filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Impossible d'ouvrir le fichier BMP : " << filename << std::endl;
    return false;
  }

  // Lire l'en-tête du fichier (54 octets)
  unsigned char data_header[54];

  file.read(reinterpret_cast<char*>(data_header), 54);
  analyseHeader(data_header);
  // Vérifier la signature "BM"
  if (header->sign != 0x424D) {
    std::cerr << "Ce n'est pas un fichier BMP valide." << std::endl;
    return false;
  }

  // Allouer le vecteur pour stocker les pixels
  pixels.resize(infos->imageSize);

  // Lire les pixels bruts du fichier
  file.read(reinterpret_cast<char*>(pixels.data()), infos->imageSize);
  file.close();

  //[ASC] reorder the pixels
  std::vector<unsigned char> line;
  std::vector<unsigned char> tmp = pixels;
  pixels.clear();
  line.clear();

  for(unsigned int i = 0; i <= infos->height; i++) {
    std::vector<unsigned char>::iterator bol = tmp.begin();
    std::vector<unsigned char>::iterator eol = tmp.begin();
    if(i > 0)
      advance(bol, (i-1)*infos->width*3); // les trois couleurs sont codees sur 24 bits
    advance(eol, i*infos->width*3);
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

unsigned int image::BMP::width() {return infos->width;}
unsigned int image::BMP::height() {return infos->height;}
const unsigned char * image::BMP::data() {return pixels.data();}
