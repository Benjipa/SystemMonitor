
#include <unistd.h>
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "processor.h"
#include "linux_parser.h"

using std::cout;
using std::cin;
using std::endl;
using std::stoi;
using std::string;
using LinuxParser::kProcDirectory;
using LinuxParser::kStatFilename;

/* Positions in /proc/stat on "cpu" line */
static const int USER       = 1; /* Non Idle time */
static const int NICE       = 2; /* Non Idle time */
static const int SYSTEM     = 3; /* Non Idle time */
static const int IDLE       = 4; /* Idle time */
static const int IOWAIT     = 5; /* Idle time */
static const int IRQ        = 6; /* Non Idle time */
static const int SOFTIRQ    = 7; /* Non Idle time */
static const int STEAL      = 8; /* Non Idle time */

/*
    Calculate and return CPU Utilization
*/
float Processor::Utilization()
{
    string line;
    string cpu;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            while(linestream)
            {
                linestream >> cpu;
                if(cpu == "cpu")
                {
                    return this->CalcUtilization(line);
                }
            }
        }
    }
    else
    {
        cout << "File NOT opened" << endl;
    }
    return 0.0;
}

/* 
    Private member method of Processor class
    Calculates the utilization and returns to Utilization()
*/
float Processor::CalcUtilization(std::string cpuLine)
{
    string data;
    int count{0};

    float currentIdle{0.0};
    float previousIdle{0.0};
    float currentNonIdle{0.0};
    float previousNonIdle{0.0};
    float currentTotal{0.0};
    float previousTotal{0.0};

    float deltaTotal{0.0};
    float deltaIdle{0.0};
    float cpuUsage{0.0};

    /* Store previous values */
    this->previousUser = this->user;
    this->previousNice = this->nice;
    this->previousSystem = this->system;
    this->previousIdle = this->idle;
    this->previousIowait = this->iowait;
    this->previousIrq = this->irq;
    this->previousSoftirq = this->softirq;
    this->previousSteal = this->steal;

    /* Get current values and update current member variables */
    std::istringstream linestream(cpuLine);
    while(linestream >> data)
    {
        if(count == USER)   { this->user      = stoi(data); }
        if(count == NICE)   { this->nice      = stoi(data); }
        if(count == SYSTEM) { this->system    = stoi(data); }
        if(count == IDLE)   { this->idle      = stoi(data); }
        if(count == IOWAIT) { this->iowait    = stoi(data); }
        if(count == IRQ)    { this->irq       = stoi(data); }
        if(count == SOFTIRQ){ this->softirq   = stoi(data); }
        if(count == STEAL)  { this->steal     = stoi(data); }

        count++;
    }

    /* Current and previous Idle times */
    previousIdle = this->previousIdle + this->previousIowait;
    currentIdle = this->idle + this->iowait;

    /* Current and previous NON Idle times */
    previousNonIdle = this->previousUser + this->previousNice + this->previousSystem + this->previousIrq + this->previousSoftirq + this->previousSteal;
    currentNonIdle = this->user + this->nice + this->system + this->irq + this->softirq + this->steal;
    
    /* Current and previous total times */
    previousTotal = previousIdle + previousNonIdle;
    currentTotal = currentIdle + currentNonIdle;

    /* Total and Idle delta times */
    deltaTotal = currentTotal - previousTotal;
    deltaIdle = currentIdle - previousIdle;

    /* Calculate the actual CPU usage in percentage */
    cpuUsage = (deltaTotal - deltaIdle) / deltaTotal;

    return cpuUsage;
}