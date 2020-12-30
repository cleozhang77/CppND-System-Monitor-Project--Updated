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

//Return this process's ID
int Process::Pid() { 
  return pid_; 
}

//Return this process's CPU utilization
float Process::CpuUtilization() const {
  long act = LinuxParser::ActiveJiffies(pid_);
  // Porcess uptime return in Jiffies for a more accurate calculation
  long total = LinuxParser::UpTime(pid_);
  return (float) act / (float) total;
}

//Return the command that generated this process
//Return the cmd with length less than 40, add "..." when returning longer cmd.
string Process::Command() const { 
  string cmd = LinuxParser::Command(pid_);
  if(cmd.size() <= 40){
    return cmd;
  } else {
    return cmd.substr(0, 39) + "...";
  }
}

//Return this process's memory utilization
string Process::Ram() const{ 
  return LinuxParser::Ram(pid_);
}

//Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid_);

}

//Return the age of this process (in seconds)
long int Process::UpTime() const { 
  // Convert process uptime in Jiffies into seconds
  return LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

//Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return CpuUtilization() < a.CpuUtilization();
}
