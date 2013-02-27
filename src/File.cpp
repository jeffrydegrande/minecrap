#include "File.h"
#include <string>
#include <fstream>
#include <sstream>

std::string File::readText(const char * path) {
   std::ifstream ifs(path);
   // if (!ifs)

   std::stringstream buffer;
   buffer << ifs.rdbuf();
   ifs.close();

   const std::string &s = buffer.str();
   return s;
}
