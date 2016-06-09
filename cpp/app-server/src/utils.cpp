//
// Created by root on 09.06.16.
//

#include "utils.h"

time_t utils::getActualUTCTime() {
    time_t actualTime;
    time(&actualTime);
    tm* act = localtime(&actualTime);
    actualTime = timegm(act);
    return  actualTime;
}