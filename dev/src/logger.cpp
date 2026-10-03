#include "logger.h"

//private:
void Logger::console (const char * message) {
  std::cout << message;
}

void Logger::file(const char * message) {
}

//public:
Logger::Logger() {
}

void Logger::log (const char * message) {
  console(message);
}

Logger& Logger::operator<<(const std::string message) {
  console(message.c_str());
  return *this;
}

Logger& Logger::operator<<(const int value) {
  char* s = new char[256];
  sprintf(s, "%d", value);
  console(s);
  return *this;
}
