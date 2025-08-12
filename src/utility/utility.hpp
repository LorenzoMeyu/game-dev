#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <string>

class Utility {
public:
  Utility();
  ~Utility();

  static void Log(const std::string &message);
};
#endif