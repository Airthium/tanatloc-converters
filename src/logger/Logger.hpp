#ifndef _LOGGER_
#define _LOGGER_

#include <string>

class Logger {
public:
  static void DEBUG(const std::string &);
  static void LOG(const std::string &);
  static void DISP(const std::string &);
  static void WARNING(const std::string &);
  static void ERROR(const std::string &);
};

#endif //_LOGGER_
