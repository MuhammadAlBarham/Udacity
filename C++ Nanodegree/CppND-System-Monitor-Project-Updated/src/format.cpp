#include "format.h"

#include <string>

using std::string;
using std::to_string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hour = seconds / 3600;
  seconds = seconds % 3600;
  long min = seconds / 60;
  seconds = seconds % 60;
  long sec = seconds;

  string time_h = hour < 10 ? '0' + to_string(hour) : to_string(hour);
  string time_m = min < 10 ? '0' + to_string(min) : to_string(min);
  string time_s = sec < 10 ? '0' + to_string(sec) : to_string(sec);

  string total = time_h + ':' + time_m + ':' + time_s;

  return total;
}