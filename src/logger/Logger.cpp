#include "./Logger.hpp"

#include <iostream>

void Logger::DEBUG(const std::string &message) {
#ifdef DEBUG_MODE
  try {
    std::cout << "\033[1;34m";
    std::cout << "DEBUG: ";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
    std::cout.flush();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
#endif
}

void Logger::LOG(const std::string &message) {
  try {
    std::cout << message << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Logger::DISP(const std::string &message) {
  try {
    std::cout << message << std::endl;
    std::cout.flush();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Logger::WARNING(const std::string &message) {
  try {
    std::cout << "\033[1;33m";
    std::cout << "WARNING: ";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
    std::cout.flush();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Logger::ERROR(const std::string &message) {
  try {
    std::cerr << "\033[1;31m";
    std::cerr << "ERROR: ";
    std::cerr << message;
    std::cerr << "\033[0m" << std::endl;
    std::cerr.flush();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
