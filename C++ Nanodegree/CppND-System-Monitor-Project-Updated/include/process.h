#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>

#include <cctype>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
const std::string kPasswordPath{"/etc/passwd"};

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                   // TODO: See src/process.cpp
  std::string User();          // TODO: See src/process.cpp
  std::string Command();       // TODO: See src/process.cpp
  float CpuUtilization();      // TODO: See src/process.cpp
  std::string Ram();           // TODO: See src/process.cpp
  long int UpTime();           // TODO: See src/process.cpp
  bool operator<(Process& a);  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  float cpu_;
};

#endif