#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::replace;
using LinuxParser::kVersionFilename;
using LinuxParser::kProcDirectory;
using LinuxParser::kOSPath;
using LinuxParser::kStatFilename;
using LinuxParser::kUptimeFilename;

#define KERNEL  ((int)2)

// TODO: Return the system's CPU
Processor& System::Cpu()
{
    Processor cpu;
    return cpu;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes()
{
    Process process;

    /* Get each PID, stored in a vector */
    vector<int> pidVec = LinuxParser::Pids(); //use this

    /* Initialize process_ vector */
    for(int i = 0; i < pidVec.size(); i++)
    {
        int pid = pidVec[i];
        processes_.push_back(process);

        /* Store PID */
        processes_[i].setPid(pid);
    }

    std::sort(processes_.begin(), processes_.end());
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel()
{
    int count = 0;
    string line;
    string kernel;
    std::ifstream filestream(kProcDirectory + kVersionFilename);
    if(filestream.is_open())
    {
        getline(filestream, line);
        std::istringstream linestream(line);
        while(linestream)
        {
            linestream >> kernel;
            if(count == KERNEL) // Return Kernel version (the third string in the line)
            {
                return kernel;
            }
            count++;
        }
    }
    else
    {
        cout << "File NOT opened" << endl;
    }
    return kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// TODO: Return the operating system name
std::string System::OperatingSystem()
{
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {  
            replace(line.begin(), line.end(), ' ', '_');
            replace(line.begin(), line.end(), '=', ' ');
            replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while(linestream >> key >> value)
            {
                if(key == "PRETTY_NAME")
                {
                    replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    else
    {
        std::cout << "File NOT opened" << std::endl;
    }
    return value;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses()
{
    string line;
    string key;
    int value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            if(linestream >> key >> value)
            {
                if(key == "procs_running")
                {
                    return value;
                }
            }
        }
    }
    return value;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses()
{
    string line;
    string key;
    int value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            if(linestream >> key >> value)
            {
                if(key == "processes")
                {
                    return value;
                }
            }
        }
    }
    return value;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime()
{
    string line;
    long int uptime;
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            if(linestream >> uptime)
            {
                // cout << uptime << endl;
                return uptime;
            }
        }
    }
    return uptime;
}