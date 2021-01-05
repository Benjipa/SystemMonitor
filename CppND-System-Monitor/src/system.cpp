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
using LinuxParser::kMeminfoFilename;

#define KERNEL  ((int)2)

/*
    Returns a reference to Processor object.
*/
Processor& System::Cpu()
{
    return cpu_;
}

/*
    Creates a process object and adds it to the process_ vector
*/
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

/*
    Returns the systems kernel information
*/
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

/*
    Returns the systems memory utilization
*/
float System::MemoryUtilization()
{
    int ram{0};
    string line;
    string parameter;
    int data{0};
    float memTotal{0.0};
    float memFree{0.0};
    float totalMemoryUsed{0.0};

    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            if(linestream >> parameter >> data)
            {
                if(parameter == "MemTotal:")
                {
                    memTotal = data/1000;
                }

                if(parameter == "MemFree:")
                {
                    memFree = data/1000;
                }
            }
        }
    }

    totalMemoryUsed = memTotal - memFree;
    return totalMemoryUsed;
}

/*
    Returns the operating system name
*/
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

/*
    Returns the number of processes actively running on the system
*/
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

/*
    Return the total number of processes on the system
*/
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

/*
    Return the number of seconds since the system started running
*/
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
                return uptime;
            }
        }
    }
    return uptime;
}