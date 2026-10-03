#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

class Logger {
  private:
    void console (const char * message);
    void file(const char * message);

  public:
    Logger();
    void log (const char * message);
    Logger& operator<<(const std::string message);
    Logger& operator<<(const int value);
};

#endif
