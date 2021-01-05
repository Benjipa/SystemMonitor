#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"
#include "system.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::vector;
using LinuxParser::kProcDirectory;
using LinuxParser::kStatFilename;

#define USER_TIME_POSITION          ((unsigned int)14)
#define KERNEL_TIME_POSITION        ((unsigned int)15)
#define WAIT_USER_TIME_POSITION     ((unsigned int)16)
#define WAIT_KERNEL_TIME_POSITION   ((unsigned int)17)
#define START_TIME_POSITION         ((unsigned int)22)

/*
    Return this process's ID
*/
int Process::Pid()
{
    return this->pid;
}

 void Process::setPid(int pid)
 {
     this->pid = pid;
 }

 void Process::setUser(std::string user)
 {
     this->user = user;
 }

/*
    Return this process's CPU utilization
*/
float Process::CpuUtilization() const
{
    string line;
    string PID = to_string(this->pid);
    float userTime;
    float kernelTime;
    float waitUserTime;
    float waitKernelTime;
    float startTime;
    long int upTime;
    float totalTime;
    int seconds;
    string data;
    int count{1};
    float cpuUsage{0.0};

    System system;
    upTime = system.UpTime();

    std::ifstream filestream(kProcDirectory + PID + kStatFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            while(linestream >> data)
            {
                /* Get each parameter from different positions in the file */
                if(count == USER_TIME_POSITION){ userTime = float(stoi(data)); }
                if(count == KERNEL_TIME_POSITION){ kernelTime = float(stoi(data)); }
                if(count == WAIT_USER_TIME_POSITION){ waitUserTime = float(stoi(data)); }
                if(count == WAIT_KERNEL_TIME_POSITION){ waitKernelTime = float(stoi(data)); }
                if(count == START_TIME_POSITION)
                {
                    startTime = float(stoi(data));
                    break;
                }
                count++;
            }
        }
    }

    totalTime = userTime + kernelTime;
    totalTime += waitUserTime + waitKernelTime;

    seconds = upTime - (startTime/sysconf(_SC_CLK_TCK));
    cpuUsage = ((totalTime/sysconf(_SC_CLK_TCK)) / seconds);

    return cpuUsage;
}

string Process::Command()
{
    return LinuxParser::Command(this->pid);
}

/*
    Return this process's memory utilization
*/
string Process::Ram()
{
    return LinuxParser::Ram(this->pid);
}

/*
    Return the user (name) that generated this process
*/
string Process::User()
{
    string uid = LinuxParser::Uid(this->pid);
    return LinuxParser::User(uid);
}

/*
    Return the age of this process (in seconds)
*/
long int Process::UpTime()
{
    return LinuxParser::UpTime(this->pid);
}

/*
    Overload the "less than" comparison operator for Process objects
*/
bool Process::operator<(Process const& a) const
{
    return (a.CpuUtilization() < this->CpuUtilization());
}