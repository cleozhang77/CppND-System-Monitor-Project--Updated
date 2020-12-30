#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename){
  string line;
  string key;
  T value;
  
  std::ifstream filestream(LinuxParser::kProcDirectory + filename);
  if(filestream.is_open()){
    
    while(getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == keyFilter){
          filestream.close();
          return value;
        }
      }
    }
  }
  return value;
}

template <typename T>
T getValueOfFile(std::string const &filename){
  std::string line;
  T value;
  std::ifstream filestream(LinuxParser::kProcDirectory + filename);
  if(filestream.is_open()){
    getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    filestream.close();
  }
  
  return value;
}


// DONE: An example of how to read data from the filesystem
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
          filestream.close();
          return value;
        }
      }
    }
    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
  }
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

float LinuxParser::MemoryUtilization() { 
  float mem_total = findValueByKey<float>(filterMemTotalString, kMeminfoFilename);
  float mem_free = findValueByKey<float>(filterMemFreeString, kMeminfoFilename);
  return 1 - (mem_free / mem_total);
}

long LinuxParser::UpTime() { 
  long uptime = getValueOfFile<long>(kUptimeFilename);
  return uptime;
}

long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line;
  long total = 0;
  string value;
  if(filestream.is_open()) {
    getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    for(int i = 0; i < 22; i++){
      linestream >> value;
      if(i == 13 || i == 14 || i == 15 || i == 16){
        total += stol(value);
      }
    }
    filestream.close();
  }
  return total;
}

long LinuxParser::ActiveJiffies() {
  long total{0};
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  string category;
  std::ifstream filestream{kProcDirectory + kStatFilename};
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> category;
      if(category.rfind(filterCpuString, 0) == 0){
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        total += user + nice + system;
      }
    }
    filestream.close();
  }
  return total;
}

long LinuxParser::IdleJiffies() {
  long total{0};
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  string category;
  std::ifstream filestream{kProcDirectory + kStatFilename};
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> category;
      if(category.rfind(filterCpuString, 0) == 0){
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        total += idle + iowait;
      }
    }
    filestream.close();
  }
  return total;
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> result {};
  string line;
  string category;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream filestream{kProcDirectory + kStatFilename};
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> category;
      if(category.rfind(filterCpuString, 0) == 0){
		linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        float act = user + nice + system;
        float idl = idle + iowait;
        float per = act / (act + idl);
        result.push_back(std::to_string(per));
      }
    }
    filestream.close();
  }
  return result;
}

int LinuxParser::TotalProcesses() { 
  int processes = findValueByKey<int>(filterProcessesString, kStatFilename);
  return processes;
}

int LinuxParser::RunningProcesses() { 
  int running = findValueByKey<int>("procs_running", kStatFilename);
  return running;
}

string LinuxParser::Command(int pid) { 
  string cmd = getValueOfFile<string>(std::to_string(pid) + kCmdlineFilename);
  return cmd;
}

//Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  // Using VmData instead of VmSize for processmemory usage
  // According to reviewer, VmSize is the sum of all the virtual memory, which give memory usage more than physical RAM size!
  // Whereas when you use VmData then it gives the exact physical memory being used as a part of Physical RAM. 
  // So it is recommended to replace the string VmSize with VmData.
  long kbsize = findValueByKey<long>(filterVmDataString, std::to_string(pid) + kStatusFilename);
  long mbsize = kbsize / 1000;
  return std::to_string(mbsize);
}

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {   
  string uid = findValueByKey<string>(filterUidString, std::to_string(pid) + kStatusFilename);
  return uid;
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  string line;
  string user, x, id;
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> id;
      if(id == uid){
        return user;
      }
    }
    filestream.close();
  }
  return string();  
}

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {   
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line;
  string value;
  if(filestream.is_open()) {
    getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    // the 22nd value is the start time of this process
    for(int i = 0; i < 22; i++){
      linestream >> value;
    }
    filestream.close();
    return UpTime() * sysconf(_SC_CLK_TCK) - stol(value); 
  }
  return 0; 
}