#include <iostream>
#include <vector>
#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include "process.h"

int main() {
  System system;
  NCursesDisplay::Display(system);
  //for(auto p : system.Processes()){
  // std::cout<< p.CpuUtilization() << std::endl;
  //}
  //std::cout << LinuxParser::TotalProcesses() << std::endl;
  //std::cout << LinuxParser::MemoryUtilization() << std::endl;
}