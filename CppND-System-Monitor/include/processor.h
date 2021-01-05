#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

 private:
    float CalcUtilization(std::string cpuLine); 
    float utilization{0};

    /* Previous values */
    int previousUser = 0;       /* Position 1 */
    int previousNice = 0;       /* Position 2 */
    int previousSystem = 0;     /* Position 3 */
    int previousIdle = 0;       /* Position 4 */
    int previousIowait = 0;     /* Position 5 */
    int previousIrq = 0;        /* Position 6 */
    int previousSoftirq = 0;    /* Position 7 */
    int previousSteal = 0;      /* Position 8 */

    /* Current values */
    int user = 0;       /* Position 1 */
    int nice = 0;       /* Position 2 */
    int system = 0;     /* Position 3 */
    int idle = 0;       /* Position 4 */
    int iowait = 0;     /* Position 5 */
    int irq = 0;        /* Position 6 */
    int softirq = 0;    /* Position 7 */
    int steal = 0;      /* Position 8 */
};

#endif