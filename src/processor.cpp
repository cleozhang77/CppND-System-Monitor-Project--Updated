#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
// Use float, also can change to double for better precision
float Processor::Utilization() { 
  prevact = curact;
  prevtotal = curtotal;
  curact = LinuxParser::ActiveJiffies();
  curtotal = LinuxParser::Jiffies();
  
  float deltatotal = curtotal - prevtotal;
  float deltaact = curact - prevact;
  return deltaact / deltatotal;
}