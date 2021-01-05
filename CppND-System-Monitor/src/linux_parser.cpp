#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::cout;
using std::cin;
using std::endl;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);  
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }


string LinuxParser::Command(int pid)
{
  string PID = to_string(pid);
  string line;

  std::ifstream filestream(kProcDirectory + PID + kStatusFilename);
  if(filestream.is_open())
  {
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
    string PID = to_string(pid);
    int ram{0};
    string line;
    string data;
    string ramSize;
    std::ifstream filestream(kProcDirectory + PID + kStatusFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            if(linestream >> data >> ramSize)
            {
                if(data == "VmSize:")
                {
                    ram = stoi(ramSize)/1000;
                    return to_string(ram);
                }
            }
        }
    }
  return ramSize;
}

/*
  Get User ID
*/
string LinuxParser::Uid(int pid)
{
    string PID = to_string(pid);
    string line;
    string uid;
    string actualUid;
    std::ifstream filestream(kProcDirectory + PID + kStatusFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            if(linestream >> uid >> actualUid)
            {
                if(uid == "Uid:")
                {
                    return actualUid;
                }
            }
        }
    }
    return actualUid;
}

/*
  Get User Name associated with a User ID
*/
string LinuxParser::User(std::string UID)
{
    string line;
    string user;
    string x;
    string uid;
    std::ifstream filestream(kPasswordPath);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            if(linestream >> user >> x >> uid)
            {
                if(uid == UID)
                {
                    return user;
                }
            }
        }
    }
    return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid)
{
    string PID = to_string(pid);
    string line;
    string uptime;
    int count{0};

    std::ifstream filestream(kProcDirectory + PID + kStatFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            while(linestream >> uptime)
            {
                if(count == 22)
                {
                  int time = stoi(uptime);
                  // cout << "upTime = " << (time/sysconf(_SC_CLK_TCK)) << endl;
                  return (time/sysconf(_SC_CLK_TCK));
                }
                count++;
            }
        }
    }
    return 0;
}