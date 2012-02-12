#ifndef LOG_H
#define LOG_H

#include <string>
#include <sstream>
#include "vec2d.h"
#include "segment.h"

namespace Log
{
  enum Level { LOGLEVEL_NONE, LOGLEVEL_ERROR, LOGLEVEL_WARNING, LOGLEVEL_INFO, LOGLEVEL_DEBUG };
  
  class Logger
  {
  public:
    Logger(Level const& level);
    Logger(Logger const& other);
    ~Logger();
    Logger& operator<<(char const* const val);
    Logger& operator<<(int const& val);
    Logger& operator<<(unsigned int const& val);
    Logger& operator<<(long unsigned int const& val);
    Logger& operator<<(float const& val);
    Logger& operator<<(Vec2D const& val);
    Logger& operator<<(Segment const& val);
  private:
    std::ostringstream content;
    Level level;
  };
  
  void log(Level const& level, std::string const& message);
  
  void debug(std::string const& message);
  void info(std::string const& message);
  void warning(std::string const& message);
  void error(std::string const& message);

  Logger debug();
  Logger info();
  Logger warning();
  Logger error();

  void setLevel(Level const& level);
}

#endif