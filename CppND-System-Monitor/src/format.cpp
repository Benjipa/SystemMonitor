#include <string>
#include <iostream>
#include "format.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;

/*
    Converts seconds to format HH:MM:SS
*/
string Format::ElapsedTime(long seconds)
{
    int hh{0};
    int mm{0};
    int ss{0};
    string hoursString{};
    string minutesString{};
    string secondsString{};
    int checkHours = seconds / 3600;
    if(checkHours >= 1)
    {
        hh = seconds / 3600; // 1,018
        int remainderSeconds = seconds % 3600; // 65
        mm = remainderSeconds / 60;
        ss = remainderSeconds % 60;
    }
    else
    {
        hh = 0;
        mm = seconds / 60;
        ss = seconds % 60;
    }
    
    /* Convert from int to string */
    hoursString = to_string(hh);
    minutesString = to_string(mm);
    secondsString = to_string(ss);

    /* Make sure there is a "0" in front of single digit */
    hoursString = hh < 10 ? "0" + hoursString : hoursString;
    minutesString = mm < 10 ? "0" + minutesString : minutesString;
    secondsString = ss < 10 ? "0" + secondsString : secondsString;

    return hoursString + ":" + minutesString + ":" + secondsString;
}