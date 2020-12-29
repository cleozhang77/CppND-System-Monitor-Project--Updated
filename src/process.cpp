#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>


#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid) : pid_(pid){}

// TODO: Return this process's ID
int Process::Pid() { 
  return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  long act = LinuxParser::ActiveJiffies(pid_);
  // Porcess uptime return in Jiffies for a more accurate calculation
  long total = LinuxParser::UpTime(pid_);
  return (float) act / (float) total;
}

// TODO: Return the command that generated this process
string Process::Command() const { 
  return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() const{ 
  return LinuxParser::Ram(pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid_);

}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { 
  // Convert process uptime in Jiffies into seconds
  return LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return CpuUtilization() < a.CpuUtilization();
}
