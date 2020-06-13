#include "processor.h"

#include "linux_parser.h"

// Define a constructor

Processor::Processor() {
  all_vl = LinuxParser::Jiffies();
  idle_vl = LinuxParser::IdleJiffies();
}

//Return the aggregate CPU utilization

float Processor::Utilization() {
  float old_all = all_vl;
  float old_idle = idle_vl;

  all_vl = LinuxParser::Jiffies();
  idle_vl = LinuxParser::IdleJiffies();

  float utlization =
      ((all_vl - old_all) - (idle_vl - old_idle)) / (all_vl - old_all);

  return (utlization > 0.0) ? utlization : 0.0;


}