#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int h = seconds / 3600;
  seconds = seconds % 3600;
  int m = seconds / 60;
  seconds = seconds % 60;
  return string(std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(seconds)); 
}