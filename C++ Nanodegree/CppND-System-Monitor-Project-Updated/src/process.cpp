#include "process.h"

#include <unistd.h>

#include <cctype>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  long tot_time = LinuxParser::ActiveJiffies(pid_);
  float seconds = LinuxParser::UpTime() - UpTime();

  return (seconds != 0) ? (cpu_ = tot_time / sysconf(_SC_CLK_TCK)) / seconds
                        : cpu_ = 0;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

//  Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects

bool Process::operator<(Process &a) {
  return CpuUtilization() < a.CpuUtilization();
}