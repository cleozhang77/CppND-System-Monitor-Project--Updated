#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;

//Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  // Clean vector processes everytime when called, to avoid push duplicate process into this vector
  processes_ = {};
  for(auto pid : pids){
    processes_.push_back(pid);
  }
  // sort the processes vector in acsending order
  std::sort(processes_.begin(), processes_.end(), [](Process& a, Process& b){return b<a;});
  
  return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel(); 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  // Time unit is second
  return LinuxParser::UpTime();
}