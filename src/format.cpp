#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
  int h = seconds / 3600;
  string hour = h < 10 ? std::to_string(h).insert(0, 1, '0') : std::to_string(h);
  
  seconds = seconds % 3600;
  int m = seconds / 60;
  string minute = m < 10 ? std::to_string(m).insert(0, 1, '0') : std::to_string(m);
  seconds = seconds % 60;
  string second = seconds < 10 ? std::to_string(seconds).insert(0, 1, '0') : std::to_string(seconds);
  return string(hour + ":" + minute + ":" + second); 
}