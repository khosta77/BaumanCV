#ifndef CVRANGEFINDER_STIME_H_
#define CVRANGEFINDER_STIME_H_

#include <iostream>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <unistd.h>
#include <ctime>
#include <chrono>

struct STime {
    int h;
    int m;
    int s;

    STime() = default;
    STime(const int&H, const int&M, const int&S) : h(H), m(M), s(S) {}

    void getSystemTime();

    friend std::ostream &operator<< (std::ostream& os, const STime& st);
};


#endif // CVRANGEFINDER_STIME_H_
