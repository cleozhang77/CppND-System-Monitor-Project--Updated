#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  prevact = curact;
  prevtotal = curtotal;
  curact = LinuxParser::ActiveJiffies();
  curtotal = LinuxParser::Jiffies();
  
  float deltatotal = curtotal - prevtotal;
  float deltaact = curact - prevact;
  return deltaact / deltatotal;
}