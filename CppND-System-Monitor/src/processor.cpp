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
using std::string;
using LinuxParser::kProcDirectory;
using LinuxParser::kStatFilename;

// TODO: Return the aggregate CPU utilization
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
                if(cpu == "cpu") // Return Kernel version (the third string in the line)
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
    return this->utilization;
}

/* Private member method of Processor class
   Calculates the utilization and returns to Utilization() */
float Processor::CalcUtilization(std::string cpuLine)
{
    // cout << cpuLine << endl;
    return 0.0;
}