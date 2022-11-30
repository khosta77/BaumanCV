#ifndef CVRANGEFINDER_TEST_H_
#define CVRANGEFINDER_TEST_H_

#include "Mat.h"
#include "correlation.h"
#include "rangefinder.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

#if defined(_WIN32)  // автор метода ниже: https://habr.com/ru/post/282301/
#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>

#else
#error "Unable to define getCPUTime( ) for an unknown OS."
#endif

/**
 * Returns the amount of CPU time used by the current process,
 * in seconds, or -1.0 if an error occurred.
 */
double getCPUTime( )
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    if ( GetProcessTimes( GetCurrentProcess( ),
        &createTime, &exitTime, &kernelTime, &userTime ) != -1 )
    {
        SYSTEMTIME userSystemTime;
        if ( FileTimeToSystemTime( &userTime, &userSystemTime ) != -1 )
            return (double)userSystemTime.wHour * 3600.0 +
                (double)userSystemTime.wMinute * 60.0 +
                (double)userSystemTime.wSecond +
                (double)userSystemTime.wMilliseconds / 1000.0;
    }

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */

#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
    /* Prefer high-res POSIX timers, when available. */
    {
        clockid_t id;
        struct timespec ts;
#if _POSIX_CPUTIME > 0
        /* Clock ids vary by OS.  Query the id, if possible. */
        if ( clock_getcpuclockid( 0, &id ) == -1 )
#endif
#if defined(CLOCK_PROCESS_CPUTIME_ID)
            /* Use known clock id for AIX, Linux, or Solaris. */
            id = CLOCK_PROCESS_CPUTIME_ID;
#elif defined(CLOCK_VIRTUAL)
            /* Use known clock id for BSD or HP-UX. */
            id = CLOCK_VIRTUAL;
#else
            id = (clockid_t)-1;
#endif
        if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
            return (double)ts.tv_sec +
                (double)ts.tv_nsec / 1000000000.0;
    }
#endif

#if defined(RUSAGE_SELF)
    {
        struct rusage rusage;
        if ( getrusage( RUSAGE_SELF, &rusage ) != -1 )
            return (double)rusage.ru_utime.tv_sec +
                (double)rusage.ru_utime.tv_usec / 1000000.0;
    }
#endif

#if defined(_SC_CLK_TCK)
    {
        const double ticks = (double)sysconf( _SC_CLK_TCK );
        struct tms tms;
        if ( times( &tms ) != (clock_t)-1 )
            return (double)tms.tms_utime / ticks;
    }
#endif

#if defined(CLOCKS_PER_SEC)
    {
        clock_t cl = clock( );
        if ( cl != (clock_t)-1 )
            return (double)cl / (double)CLOCKS_PER_SEC;
    }
#endif

#endif

    return -1;      /* Failed. */
}

#define CAMERA_TEST 0

#define IDENTICAL_PHOTOS 1
#define DEFFERENT_PHOTOS 2
#define DEFFERENT_ANGLES 3
#define BLUR 4
#define CORRELATION_TIME 5
#define RGB_TO_GRAY 6


#define DISTANSE 9

#define TEST_PERIOD 20  /* Колличество тестов в 1 цикле */

void runTest(int, const char**);
int getTestCode(const std::string&);

void identicalPhotosTest();
void defferentPhotosTest();
void defferentAngelsTest();
void blurTest();
void correlationTimeTest();
void rgbToGrayTest();


void printDistance();

class MyTest {
    struct test_obj {
        std::string name;
        void (*foo_ptr)();

        test_obj(const std::string n, const void (*fPtr)()) : name(n), foo_ptr(fPtr) {} 
    };

    struct test_result {
        std::string name;
        double average_value;
        double min_value;
        double max_value;
        std::vector<double> all_value;
    };

    static std::vector<test_result> results;  // Результаты тестов

    static std::vector<test_obj> tests_methods;  // Переменная в которой хранится вектор тестов
    static size_t TEST_COUNT = 10;
public:
    MyTest() = default;
    ~MyTest() = default;

    static void add(const std::string tn, const void (*tfPtr)()) {
        tests_methods.push_back(test_obj(tn, tfPtr));
    }

    // TODO: радикольно уменьшить размер этой функции, вынеся ее куски в отдельные методы 
    static void make() {
        time_t now = time(0);
        char* start = ctime(&now);
        for (size_t i = 0; i < tests_methods.size(); ++i) { // вот это сбросить в метод отдельный
            double startTime, endTime;
            test_result tr;
            for (size_t j = 0; j < TEST_COUNT; ++j) {  // вот это в отдельный метод
                startTime = getCPUTime();
                tests_methods[i].foo_ptr();
                endTime = getCPUTime();
                tr.all_value.push_back((endTime - startTime));
            }
            tr.average_value = tr.all_value[0];
            for (size_t j = 1; j < tr.all_value.size(); ++j)  // в отдельный методд
                tr.average_value += tr.all_value[j]; 
            tr.average_value /= tr.all_value.size();
            tr.min_value = *std::min_element(tr.all_value.begin(), tr.all_value.end());
            tr.max_value = *std::max_element(tr.all_value.begin(), tr.all_value.end());
            tr.name = tests_methods[i].name;
            results.push_back(tr);
        }
        char* end = ctime(&now);
        // TODO: написать алгоритм который определит максимальное имя и порядок размеров, для таблицы

        // TODO: написать вывод таблицы

        // TODO: написать сохранение тестовых данных в csv файл
    }
};

#endif // CVRANGEFINDER_TEST_H_
