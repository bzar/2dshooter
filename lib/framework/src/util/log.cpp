#include "log.h"
#include <iostream>

namespace
{
  Log::Level globalLevel = Log::LOGLEVEL_INFO;
}

Log::Logger::Logger(Level const& level) : content(), level(level)
{
}

Log::Logger::Logger(Logger const& other) : content(), level(other.level)
{
}

Log::Logger::~Logger()
{
  if(!content.str().empty())
  {
    log(level, content.str());
  }
}

Log::Logger& Log::Logger::operator<< (char const* const val)
{
  if(level <= globalLevel)
  {
    content << val;
  }
  return *this;  
}

Log::Logger& Log::Logger::operator<<(int const& val)
{
  if(level <= globalLevel)
  {
    content << val;
  }
  return *this;
}

Log::Logger& Log::Logger::operator<<(unsigned int const& val)
{
  if(level <= globalLevel)
  {
    content << val;
  }
  return *this;
}

Log::Logger& Log::Logger::operator<<(long unsigned int const& val)
{
  if(level <= globalLevel)
  {
    content << val;
  }
  return *this;
}

Log::Logger& Log::Logger::operator<<(float const& val)
{
  if(level <= globalLevel)
  {
    content << val;
  }
  return *this;
}

Log::Logger& Log::Logger::operator<<(Vec2D const& val)
{
  if(level <= globalLevel)
  {
    content << "(" << val.x << ", " << val.y << ")";
  }
  return *this;
}
Log::Logger& Log::Logger::operator<<(Segment const& val)
{
  if(level <= globalLevel)
  {
    content << "(" << val.a.x << ", " << val.a.y << ")-(" << val.b.x << ", " << val.b.y << ")";
  }
  return *this;  
}

void Log::log(Level const& logLevel, std::string const& message)
{
  if(logLevel <= globalLevel)
  {
    switch(logLevel)
    {
      case LOGLEVEL_DEBUG: std::cout << "[DEBUG] " << message << std::endl; break;
      case LOGLEVEL_INFO: std::cout << "[INFO] " << message << std::endl; break;
      case LOGLEVEL_WARNING: std::cout << "[WARNING] " << message << std::endl; break;
      case LOGLEVEL_ERROR: std::cout << "[ERROR] " << message << std::endl; break;
      default: break;
    }
  }
}

void Log::debug(std::string const& message)
{
  log(LOGLEVEL_DEBUG, message);
}
void Log::info(std::string const& message)
{
  log(LOGLEVEL_INFO, message);
}
void Log::warning(std::string const& message)
{
  log(LOGLEVEL_WARNING, message);
}
void Log::error(std::string const& message)
{
  log(LOGLEVEL_ERROR, message);
}

Log::Logger Log::debug()
{
  return Logger(LOGLEVEL_DEBUG);
}
Log::Logger Log::info()
{
  return Logger(LOGLEVEL_INFO);
}
Log::Logger Log::warning()
{
  return Logger(LOGLEVEL_WARNING);
}
Log::Logger Log::error()
{
  return Logger(LOGLEVEL_ERROR);
}

void Log::setLevel(Level const& level)
{
  globalLevel = level;
}
