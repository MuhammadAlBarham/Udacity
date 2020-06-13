#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// Define a templates

template <typename T>

T findValueByKey(string const& keyfilter, string const& filename) {
  string line;
  string key;

  T value;

  std::ifstream filestream(LinuxParser::kProcDirectory + filename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyfilter) return value;
      }
    }
  }
  filestream.close();
  return value;
};

// Define a template
template <typename T>

T getValueOfFile(string const& filename) {
  string line;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  stream.close();
  return value;
};

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os;
  string version;
  string kernel;
  string line;

  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string MemTotal = "MemTotal:";
  string MemFree = "MemFree:";

  float Total = findValueByKey<float>(MemTotal, kMeminfoFilename);
  float free = findValueByKey<float>(MemFree, kMeminfoFilename);

  return (Total - free) / Total;
}

// Read and return the system uptime
long LinuxParser::UpTime() { return getValueOfFile<long>(kUptimeFilename); }

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID

long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string Utime;
  string Stime;
  string Cutime;
  string CStime;
  long jiffies;

  std::ifstream stream{kProcDirectory + to_string(pid) + kStatFilename};

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream{line};

    for (int i = 0; i < 14; i++) linestream >> Utime;

    linestream >> Stime >> Cutime >> CStime;

    jiffies = std::stol(Utime) + std::stol(Stime) + std::stol(Cutime) +
              std::stol(CStime);
  }
  stream.close();

  return jiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> active{CpuUtilization()};

  return (stol(active[CPUStates::kUser_]) + stol(active[CPUStates::kNice_]) +
          stol(active[CPUStates::kSystem_]) + stol(active[CPUStates::kIRQ_]) +
          stol(active[CPUStates::kSoftIRQ_]) +
          stol(active[CPUStates::kSteal_]) + stol(active[CPUStates::kGuest_]) +
          stol(active[CPUStates::kGuestNice_]));
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> idle_{CpuUtilization()};

  return stol(idle_[CPUStates::kIdle_]) + stol(idle_[CPUStates::kIOwait_]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string value;

  vector<string> cpu_val;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);

    std::istringstream linestream(line);

    while (linestream >> value) {
      if (value != "cpu") cpu_val.emplace_back(value);
    }
  }
  filestream.close();

  return cpu_val;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return findValueByKey<int>("processes", kStatFilename);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return findValueByKey<int>("procs_running", kStatFilename);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  return getValueOfFile<string>(to_string(pid) + kCmdlineFilename);
}

// Read and return the memory used by a process

string LinuxParser::Ram(int pid) {
  return to_string(stol(findValueByKey<string>(
                       "VmData:", to_string(pid) + kStatusFilename)) /
                   1024);
}

// Read and return the user ID associated with a process

string LinuxParser::Uid(int pid) {
  return findValueByKey<string>("Uid:", to_string(pid) + kStatusFilename);
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string key;
  string val;
  string val1;
  string check = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);
      linestream >> key >> val >> val1;

      if (val1 == check) break;
    }
  }
  filestream.close();

  return key;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string key;
  string value;
  long UpTimePid{0};

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    string v1;

    std::getline(filestream, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 22; i++) {
      linestream >> v1;
    }
    UpTimePid = UpTime() - (stol(v1) / sysconf(_SC_CLK_TCK));
  }
  filestream.close();

  return UpTimePid;
}
