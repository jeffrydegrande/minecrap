#include "File.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::string File::readText(const char * path) {
   std::ifstream ifs(path);
   if (!ifs)
       std::cerr << "Can't open file " << path << std::endl;

   std::stringstream buffer;
   buffer << ifs.rdbuf();
   ifs.close();

   const std::string &s = buffer.str();

   return s;
}
